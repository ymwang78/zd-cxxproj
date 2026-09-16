<#
============================================================================
 build.ps1  ——  xRto 一键编译 + 部署脚本
============================================================================
 编译 apps\xRto\XRto.sln，按解决方案里配置好的项目依赖决定编译顺序
 （见 apps\xRto\XRto.sln 的 ProjectDependencies），编译成功后把生成物
 拷贝到各运行目录。

 TaoTripletDir
 -------------
 CapeOpen/TAO 工程（xRtoCapeOpen / xRtoCapeOpenSolver /
 xOptMINLPcoCorbaServer / xOptMINLPcoSolverServer）需要 vcpkg 的 ace[tao]
 三元组目录（含 ACE/TAO 静态库与 tao_idl.exe）。
 默认值直接从 libsrc\xRtoCapeOpen\xRtoCapeOpenCorba.props 里解析
 （即那句 <TaoTripletDir Condition="...">...</TaoTripletDir>），命令行
 -TaoTripletDir 可覆盖，脚本以 /p:TaoTripletDir=... 传给 MSBuild。

 部署（编译成功后自动执行，-NoDeploy 可关）
 ------------------------------------------
   apps\xRto\bin\x64\xRto\            <= 主程序：XRto.exe + 一方 DLL（解决方案 bin）
   apps\xRto\bin\x64\UnitModel\Corba\ <= xRtoCapeOpen.dll + xOptMINLPcoCorbaServer.exe
   apps\xRto\bin\x64\Solver\Corba\    <= xRtoCapeOpenSolver.dll + xOptMINLPcoSolverServer.exe

 常用示例
 --------
   .\build.ps1                        # 全解决方案 Release 编译 + 部署（TAO 用 props 默认路径）
   .\build.ps1 -TaoTripletDir E:\vcpkg\installed\x64-windows-static-md
   .\build.ps1 -AppOnly               # 只编 GUI 主程序及其依赖，不碰 CapeOpen/TAO
   .\build.ps1 -Configuration Debug -Rebuild
   .\build.ps1 -Target xRtoCapeOpenSolver
============================================================================
#>
[CmdletBinding()]
param(
    # vcpkg ace[tao] 三元组目录（x64-windows-static-md）。留空时从 props 解析默认值。
    [string]$TaoTripletDir = "",

    [ValidateSet("Release", "Debug")]
    [string]$Configuration = "Release",

    [ValidateSet("x64")]
    [string]$Platform = "x64",

    # 指定要编的工程名（MSBuild target），留空 = 整个解决方案。
    [string]$Target = "",

    # 只编 GUI 主程序 XRto 及其依赖链（等价 -Target XRto），跳过 CapeOpen/TAO。
    [switch]$AppOnly,

    [switch]$Rebuild,
    [switch]$Clean,

    # 编译成功后不执行部署拷贝
    [switch]$NoDeploy,

    [int]$MaxCpuCount = 0,
    [string]$MSBuildPath = "",

    [ValidateSet("quiet", "minimal", "normal", "detailed", "diagnostic")]
    [string]$Verbosity = "minimal"
)

$ErrorActionPreference = "Stop"

function Write-Step([string]$msg) { Write-Host "==> $msg" -ForegroundColor Cyan }
function Write-Warn([string]$msg) { Write-Host "[warn] $msg" -ForegroundColor Yellow }
function Write-Err ([string]$msg) { Write-Host "[error] $msg" -ForegroundColor Red }

# --- 路径锚定：脚本所在目录即仓库根 ---------------------------------------
$RepoRoot   = $PSScriptRoot
$Solution   = Join-Path $RepoRoot "apps\xRto\XRto.sln"
$CorbaProps = Join-Path $RepoRoot "libsrc\xRtoCapeOpen\xRtoCapeOpenCorba.props"
if (-not (Test-Path $Solution)) {
    Write-Err "找不到解决方案：$Solution"
    exit 1
}

# 把工程名解析成 MSBuild 解决方案目标名：嵌在解决方案文件夹里的工程，其目标名
# 是“文件夹\...\工程名”（如 xCapeOpen\xRtoCapeOpen），根级工程就是工程名本身。
function Resolve-SlnTarget([string]$name) {
    $folderType = "2150E333-8FDC-42A3-9474-1A3956D46DE8"
    $slnText = Get-Content $Solution -Raw
    $nameByGuid = @{}; $parentByGuid = @{}; $guidByName = @{}; $isFolder = @{}
    foreach ($mm in [regex]::Matches($slnText, 'Project\("\{(?<type>[^}]+)\}"\)\s*=\s*"(?<n>[^"]+)",\s*"[^"]*",\s*"\{(?<g>[^}]+)\}"')) {
        $g = $mm.Groups['g'].Value.ToUpper()
        $nameByGuid[$g] = $mm.Groups['n'].Value
        $guidByName[$mm.Groups['n'].Value] = $g
        $isFolder[$g] = ($mm.Groups['type'].Value.ToUpper() -eq $folderType)
    }
    $nested = [regex]::Match($slnText, 'GlobalSection\(NestedProjects\).*?EndGlobalSection', 'Singleline')
    if ($nested.Success) {
        foreach ($mm in [regex]::Matches($nested.Value, '\{(?<c>[^}]+)\}\s*=\s*\{(?<p>[^}]+)\}')) {
            $parentByGuid[$mm.Groups['c'].Value.ToUpper()] = $mm.Groups['p'].Value.ToUpper()
        }
    }
    if (-not $guidByName.ContainsKey($name)) { return $name }   # 未知名字，原样返回
    $g = $guidByName[$name]
    $parts = @($name)
    $p = $parentByGuid[$g]
    while ($p -and $isFolder[$p]) {
        $parts = , $nameByGuid[$p] + $parts
        $p = $parentByGuid[$p]
    }
    return ($parts -join '\')
}

# --- TaoTripletDir 默认值：从 xRtoCapeOpenCorba.props 解析 ------------------
if (-not $TaoTripletDir) {
    if (Test-Path $CorbaProps) {
        $propsText = Get-Content $CorbaProps -Raw
        $m = [regex]::Match($propsText, '<TaoTripletDir[^>]*>(?<v>[^<]+)</TaoTripletDir>')
        if ($m.Success) {
            $TaoTripletDir = $m.Groups['v'].Value.Trim()
            Write-Step "TaoTripletDir 默认值取自 props：$TaoTripletDir"
        } else {
            Write-Warn "未能在 props 中解析 TaoTripletDir，默认值留空。"
        }
    } else {
        Write-Warn "找不到 $CorbaProps，无法解析 TaoTripletDir 默认值。"
    }
}

# --- 定位 MSBuild ----------------------------------------------------------
if (-not $MSBuildPath) {
    $vswhere = "${env:ProgramFiles(x86)}\Microsoft Visual Studio\Installer\vswhere.exe"
    if (Test-Path $vswhere) {
        $MSBuildPath = & $vswhere -latest -prerelease -products * `
            -requires Microsoft.Component.MSBuild `
            -find "MSBuild\**\Bin\MSBuild.exe" | Select-Object -First 1
    }
}
if (-not $MSBuildPath -or -not (Test-Path $MSBuildPath)) {
    Write-Err "未找到 MSBuild.exe。请安装 VS 生成工具，或用 -MSBuildPath 指定。"
    exit 1
}

# --- 处理 target -----------------------------------------------------------
if ($AppOnly -and -not $Target) { $Target = "XRto" }

$capeOpenTargets = @("xRtoCapeOpen", "xRtoCapeOpenSolver", "xOptMINLPcoCorbaServer", "xOptMINLPcoSolverServer")
$buildsCapeOpen = [string]::IsNullOrEmpty($Target) -or ($capeOpenTargets -contains $Target)

# --- 校验 TaoTripletDir -----------------------------------------------------
if ($TaoTripletDir -and (Test-Path $TaoTripletDir)) {
    $taoIdl = Join-Path $TaoTripletDir "tools\ace\tao_idl.exe"
    if (-not (Test-Path $taoIdl)) {
        Write-Warn "TaoTripletDir 下未找到 tao_idl.exe（预期 $taoIdl），CapeOpen 工程可能无法生成 IDL 桩。"
    }
} elseif ($buildsCapeOpen) {
    if (-not $TaoTripletDir) {
        Write-Warn "TaoTripletDir 为空；如需编 CapeOpen 请加 -TaoTripletDir，或用 -AppOnly 只编主程序。"
    } else {
        Write-Warn "TaoTripletDir 不存在：$TaoTripletDir"
    }
}

if ($MaxCpuCount -le 0) { $MaxCpuCount = [Environment]::ProcessorCount }

# --- 组装 MSBuild 参数 ------------------------------------------------------
$verb = if ($Clean) { "Clean" } elseif ($Rebuild) { "Rebuild" } else { "Build" }
if ($Target) {
    # 解析成解决方案目标名（含解决方案文件夹前缀）。构建动词无 :Build 后缀，
    # 目标名即工程名；Clean / Rebuild 才带 :动词 后缀。
    $slnTarget = Resolve-SlnTarget $Target
    $targets = if ($verb -eq "Build") { $slnTarget } else { "$slnTarget`:$verb" }
} else {
    $targets = $verb
}

$msbuildArgs = @(
    "`"$Solution`"",
    "/t:$targets",
    "/p:Configuration=$Configuration",
    "/p:Platform=$Platform",
    "/m:$MaxCpuCount",
    "/nologo",
    "/v:$Verbosity",
    "/p:BuildInParallel=true"
)
if ($TaoTripletDir) {
    $msbuildArgs += "/p:TaoTripletDir=`"$($TaoTripletDir.TrimEnd('\'))`""
}

# --- 打印计划并执行 ---------------------------------------------------------
Write-Step "MSBuild      : $MSBuildPath"
Write-Step "解决方案     : $Solution"
Write-Step "配置/平台    : $Configuration|$Platform"
Write-Step "目标         : $targets"
if ($TaoTripletDir) { Write-Step "TaoTripletDir: $($TaoTripletDir.TrimEnd('\'))" }
Write-Step "并行核数     : $MaxCpuCount"
Write-Host ""

$sw = [System.Diagnostics.Stopwatch]::StartNew()
& $MSBuildPath @msbuildArgs
$code = $LASTEXITCODE
$sw.Stop()

Write-Host ""
if ($code -ne 0) {
    Write-Err "MSBuild 失败，退出码 $code（用时 $([int]$sw.Elapsed.TotalSeconds)s）"
    exit $code
}
Write-Host "==> 编译成功，用时 $([int]$sw.Elapsed.TotalSeconds)s" -ForegroundColor Green

# --- 部署拷贝 ---------------------------------------------------------------
if ($Clean) { exit 0 }            # 纯 Clean 不部署
if ($NoDeploy) { Write-Step "已指定 -NoDeploy，跳过部署"; exit 0 }

# 生成物源目录（随配置变化）
$slnOut   = Join-Path $RepoRoot "apps\xRto\bin\$Platform\$Configuration"
$coreOut  = Join-Path $RepoRoot "libsrc\xRtoCapeOpen\core\bin\$Platform\$Configuration"
$minlpOut = Join-Path $RepoRoot "libsrc\xRtoCapeOpen\xOptMINLPco\bin\$Platform\$Configuration"

# 部署目标目录（固定，不带配置名）
$deployBase   = Join-Path $RepoRoot "apps\xRto\bin\$Platform"
$deployApp    = Join-Path $deployBase "xRto"
$deployUnit   = Join-Path $deployBase "UnitModel\Corba"
$deploySolver = Join-Path $deployBase "Solver\Corba"

function Ensure-Dir([string]$d) {
    if (-not (Test-Path $d)) { New-Item -ItemType Directory -Path $d -Force | Out-Null }
}

# 拷贝一个文件（只拷贝二进制本身，不带 .pdb），源缺失时告警
function Deploy-File([string]$srcFile, [string]$destDir) {
    if (Test-Path $srcFile) {
        Ensure-Dir $destDir
        Copy-Item -Path $srcFile -Destination $destDir -Force
        Write-Host "    + $(Split-Path $srcFile -Leaf)  ->  $destDir"
    } else {
        Write-Warn "缺少生成物，跳过：$srcFile"
    }
}

# 本次实际编了哪些，就只部署对应产物，避免把无关/过期文件拷进运行目录。
$full               = [string]::IsNullOrEmpty($Target)
$deployAppNeeded    = $full -or $AppOnly -or ($Target -eq 'XRto')
$deployUnitNeeded   = $full -or ($Target -eq 'xRtoCapeOpen') -or ($Target -eq 'xOptMINLPcoCorbaServer')
$deploySolverNeeded = $full -or ($Target -eq 'xRtoCapeOpenSolver') -or ($Target -eq 'xOptMINLPcoSolverServer')

# 主程序侧“一方生成物”白名单：只有这些是本解决方案产出、需要刷新的运行时二进制；
# 其余第三方 DLL（Qt6*/python312/casadi… 已在 xRto\ 中）一律不碰。
$appArtifacts = @(
    "XRto.exe",
    "Scintilla.dll",
    "Lexilla.dll",
    "libQWT.dll",
    "ScintillaEdit5.dll",
    "xOptInterface.dll",
    "ThermoLib.dll"
)

Write-Host ""
Write-Step "部署生成物"

# 1) 主程序 -> bin\x64\xRto
if ($deployAppNeeded) {
    foreach ($a in $appArtifacts) { Deploy-File (Join-Path $slnOut $a) $deployApp }
}

# 2) UnitModel/Corba：消费端模型 DLL + 问题/单元侧 CORBA server
if ($deployUnitNeeded) {
    Deploy-File (Join-Path $coreOut  "xRtoCapeOpen.dll")            $deployUnit
    Deploy-File (Join-Path $minlpOut "xOptMINLPcoCorbaServer.exe")  $deployUnit
}

# 3) Solver/Corba：求解器桥接 DLL + 求解器侧 CORBA server
if ($deploySolverNeeded) {
    Deploy-File (Join-Path $minlpOut "xRtoCapeOpenSolver.dll")        $deploySolver
    Deploy-File (Join-Path $minlpOut "xOptMINLPcoSolverServer.exe")   $deploySolver
}

Write-Host ""
Write-Host "==> 全部完成" -ForegroundColor Green
exit 0
