#pragma once

// ***************************************************************
//  XocConfirm —— 二次确认对话框的统一按钮次序
//  -------------------------------------------------------------
//  This file is a part of project KeelDeck.
// ***************************************************************
//
// 现场反馈（《安装和测试过程遇到的问题》20261020，测试问题 2）：参数修改的二次确认框
// 按 Windows 习惯把「是」放在左边，而这些框确认的都是"系统正在运行时改值"这类有后果
// 的操作，手快点了左边就改下去了。要求改成：左「否」右「是」，默认「否」。
//
// ---- 为什么不是简单地调一下 addButton 的顺序 ----
//
// QMessageBox 不按加入顺序排按钮：位置由 ButtonRole 去查 QPlatformDialogHelper 的
// 布局表决定，setDefaultButton() 只改默认焦点。而想要的「否在左」在标准角色里排不出来
// ——Windows 布局表里 YesRole 恒在 NoRole 之前。
//
// 唯一能左右次序的入口是 QDialogButtonBox 的一个内部规则：同为 AcceptRole 的按钮里，
// 只有**第一个**排在 AcceptRole 槽，第二个起改排到 AlternateRole 槽。于是先加「否」
// 后加「是」，就得到 AcceptRole=否、AlternateRole=是。
//
// 但这还不够：AlternateRole 与 AcceptRole 的先后【按平台布局表而不同】——
//   WinLayout  : … YesRole, AcceptRole, AlternateRole, … NoRole, RejectRole …  → 否 | 是
//   GnomeLayout: … DestructiveRole, AlternateRole, RejectRole, AcceptRole …    → 是 | 否  ✗
// 也就是说只按角色排，在 GNOME/Mac 上恰好翻回成「是在左」，本来要躲的误点又回来了
// （PR #404 评审实测：GnomeLayout 下 Yes.x=11、No.x=97）。AlternateRole 又只存在于
// QPlatformDialogHelper，QDialogButtonBox::ButtonRole / QMessageBox::ButtonRole 都没有
// 这个值，没法显式指定。
//
// 所以把这个框的按钮栏布局钉成 WinLayout（button-layout），让上面那条推导的结论
// 不随平台变。配套 tests/test_xoc_confirm_button_order.cpp 直接断言两个按钮的实际 x
// 坐标，Qt 哪天改了内部规则会在那里失败，而不是悄悄把次序翻过去。
//
// 按钮文字取 Qt 自己那两条（QPlatformTheme 上下文的 "&Yes" / "&No"），带不带助记符、
// 译成什么都与系统其它标准对话框一致，不另起一套文案。
//
// 纯内联：不新增虚函数也不新增导出符号，放在 interfaces/ 下不影响 XOC_PLUGIN_ABI_VERSION。

#include <QApplication>
#include <QCoreApplication>
#include <QDialogButtonBox>
#include <QMessageBox>
#include <QPushButton>
#include <QString>
#include <QWidget>

namespace xoc_confirm {

/**
 * @brief 把 box 装成一个「否 | 是」的确认框
 *
 * 与 xocConfirmYesNo() 共用，单独抽出来是为了让测试能拿到两个按钮去查实际坐标。
 *
 * @param out_no  左边那个「否」，同时是默认按钮与 Esc 按钮
 * @param out_yes 右边那个「是」
 */
inline void buildYesNoBox(QMessageBox& box, const QString& title, const QString& text,
                          QPushButton*& out_no, QPushButton*& out_yes) {
    box.setIcon(QMessageBox::Question);
    box.setWindowTitle(title);
    box.setText(text);

    // 顺序要紧：先加的「否」占 AcceptRole 槽，后加的「是」被挪到 AlternateRole 槽。
    out_no = box.addButton(QCoreApplication::translate("QPlatformTheme", "&No"),
                           QMessageBox::AcceptRole);
    out_yes = box.addButton(QCoreApplication::translate("QPlatformTheme", "&Yes"),
                            QMessageBox::AcceptRole);
    box.setDefaultButton(out_no);
    box.setEscapeButton(out_no);

    // 只覆盖布局 hint，保留应用的绘制样式和深浅色配色。无基样式的 QProxyStyle
    // 会另建系统原生样式，导致深色模式下按钮可能仍用浅色背景配浅色文字。
    // 找不到按钮栏（QMessageBox 内部结构若有变）就不钉：Windows 上的次序本来就对，
    // 退化成"只有非 Windows 平台可能翻过来"，比直接崩掉强。
    if (QDialogButtonBox* bar = box.findChild<QDialogButtonBox*>()) {
        bar->setStyleSheet(QStringLiteral("QDialogButtonBox { button-layout: %1; }")
                              .arg(QDialogButtonBox::WinLayout));
    }
}

}  // namespace xoc_confirm

/**
 * @brief 「否 | 是」次序的二次确认框，默认按钮与 Esc 都落在「否」上
 *
 * @param parent 父窗口；传 nullptr 时取当前活动窗口
 * @param title  标题栏文字
 * @param text   正文
 * @return 只有用户点了「是」才返回 true；点「否」、按 Esc、直接关窗都返回 false
 */
inline bool xocConfirmYesNo(QWidget* parent, const QString& title, const QString& text) {
    QMessageBox box(parent ? parent : qobject_cast<QWidget*>(QApplication::activeWindow()));
    QPushButton* noButton = nullptr;
    QPushButton* yesButton = nullptr;
    xoc_confirm::buildYesNoBox(box, title, text, noButton, yesButton);

    // 判定走 clickedButton() 而不是 exec() 的返回值：两个按钮都挂在 AcceptRole 上，
    // 返回值区分不了它们。关窗（clickedButton() 为空）与 Esc 一样算「否」。
    box.exec();
    return box.clickedButton() == yesButton;
}
