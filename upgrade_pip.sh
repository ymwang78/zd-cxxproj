source /opt/venv/bin/activate &&
python -m pip freeze > requirements.before-upgrade.txt &&
python -m pip install --upgrade pip setuptools wheel &&
# pydantic-core 必须与 pydantic 锁定的版本一致(pydantic 2.13.4 要求 ==2.46.4),
# 不能单独升级,否则 pip check 报依赖冲突;升级 pydantic 时会自动带上匹配的 core
python -m pip list --outdated --format=json --exclude-editable --exclude pydantic-core \
  | python -c 'import sys,json; print("\n".join(x["name"] for x in json.load(sys.stdin)))' \
  | xargs -r python -m pip install --upgrade &&
python -m pip check

