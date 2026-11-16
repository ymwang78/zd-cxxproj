#pragma once

#include <QFrame>
#include <QGroupBox>
#include <QLayout>
#include <QScrollArea>
#include <QSizePolicy>
#include <QStyle>
#include <QWidget>

namespace ControlPanelLayout {

// For stacks of form controls: keep groups at their natural height, and let
// the viewport shrink independently when the window or bottom dock is resized.
inline QScrollArea* createScrollArea(QWidget* content, QWidget* parent = nullptr) {
    for (auto* group : content->findChildren<QGroupBox*>()) {
        auto policy = group->sizePolicy();
        policy.setVerticalPolicy(QSizePolicy::Preferred);
        group->setSizePolicy(policy);
        if (auto* layout = group->parentWidget()->layout()) {
            layout->setAlignment(group, Qt::AlignTop);
        }
    }
    const auto horizontalPolicy = content->sizePolicy().horizontalPolicy();
    content->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);
    if (content->layout()) {
        content->layout()->setSizeConstraint(QLayout::SetMinimumSize);
    }

    auto* scroll = new QScrollArea(parent);
    scroll->setObjectName(QStringLiteral("controlPanelScrollArea"));
    scroll->setFrameShape(QFrame::NoFrame);
    scroll->setWidgetResizable(true);
    scroll->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    scroll->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scroll->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    scroll->setSizePolicy(horizontalPolicy, QSizePolicy::Expanding);
    scroll->setMinimumHeight(0);
    scroll->setMinimumWidth(qMax(content->minimumWidth(), content->minimumSizeHint().width()) +
                            scroll->style()->pixelMetric(QStyle::PM_ScrollBarExtent));
    scroll->setWidget(content);
    return scroll;
}

}  // namespace ControlPanelLayout
