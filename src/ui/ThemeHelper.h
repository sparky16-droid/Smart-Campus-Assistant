#ifndef THEMEHELPER_H
#define THEMEHELPER_H

#include <QString>

class ThemeHelper {
public:
    static QString getAppStyleSheet() {
        return R"(
            /* Global Application Styling */
            QWidget {
                background-color: #f8fafc;
                color: #0f172a;
                font-family: 'Segoe UI', 'Plus Jakarta Sans', Arial, sans-serif;
                font-size: 13px;
            }

            /* Scrollbars */
            QScrollBar:vertical {
                border: none;
                background: #f1f5f9;
                width: 8px;
                border-radius: 4px;
            }
            QScrollBar::handle:vertical {
                background: #cbd5e1;
                min-height: 25px;
                border-radius: 4px;
            }
            QScrollBar::handle:vertical:hover {
                background: #94a3b8;
            }
            QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {
                height: 0px;
            }

            /* Buttons */
            QPushButton {
                background-color: #1e40af;
                color: #ffffff;
                border: none;
                border-radius: 6px;
                padding: 8px 16px;
                font-weight: 600;
            }
            QPushButton:hover {
                background-color: #1d4ed8;
            }
            QPushButton:pressed {
                background-color: #1e3a8a;
            }
            QPushButton:disabled {
                background-color: #cbd5e1;
                color: #94a3b8;
            }

            /* Secondary Button */
            QPushButton#btnSecondary {
                background-color: #e2e8f0;
                color: #1e293b;
            }
            QPushButton#btnSecondary:hover {
                background-color: #cbd5e1;
            }

            /* Accent / Teal Button */
            QPushButton#btnAccent {
                background-color: #0d9488;
                color: #ffffff;
            }
            QPushButton#btnAccent:hover {
                background-color: #0f766e;
            }

            /* Quick Chip Button */
            QPushButton#chipBtn {
                background-color: #eff6ff;
                color: #1d4ed8;
                border: 1px solid #bfdbfe;
                border-radius: 14px;
                padding: 5px 12px;
                font-size: 12px;
                font-weight: 500;
            }
            QPushButton#chipBtn:hover {
                background-color: #dbeafe;
                border-color: #93c5fd;
            }

            /* Line Edit and Text Edit */
            QLineEdit, QTextEdit, QPlainTextEdit {
                background-color: #ffffff;
                border: 1px solid #cbd5e1;
                border-radius: 6px;
                padding: 8px 12px;
                color: #0f172a;
            }
            QLineEdit:focus, QTextEdit:focus, QPlainTextEdit:focus {
                border: 2px solid #2563eb;
            }

            /* Combo Box */
            QComboBox {
                background-color: #ffffff;
                border: 1px solid #cbd5e1;
                border-radius: 6px;
                padding: 6px 12px;
                min-width: 140px;
            }
            QComboBox:hover {
                border-color: #94a3b8;
            }
            QComboBox::drop-down {
                border: none;
            }

            /* Cards and Frames */
            QFrame#cardFrame {
                background-color: #ffffff;
                border: 1px solid #e2e8f0;
                border-radius: 10px;
            }

            /* Sidebar Navigation */
            QFrame#navSidebar {
                background-color: #0f172a;
                border-right: 1px solid #1e293b;
            }
            QPushButton#navBtn {
                background-color: transparent;
                color: #94a3b8;
                border: none;
                border-radius: 8px;
                text-align: left;
                padding: 10px 16px;
                font-size: 13px;
                font-weight: 500;
            }
            QPushButton#navBtn:hover {
                background-color: #1e293b;
                color: #f8fafc;
            }
            QPushButton#navBtn:checked, QPushButton#navBtn[active="true"] {
                background-color: #1e40af;
                color: #ffffff;
                font-weight: 600;
            }

            /* Tables */
            QTableWidget {
                background-color: #ffffff;
                border: 1px solid #e2e8f0;
                border-radius: 8px;
                gridline-color: #f1f5f9;
            }
            QHeaderView::section {
                background-color: #f1f5f9;
                color: #475569;
                padding: 8px;
                font-weight: 600;
                border: none;
                border-bottom: 1px solid #e2e8f0;
            }

            /* Tab Widget */
            QTabWidget::pane {
                border: 1px solid #e2e8f0;
                background: #ffffff;
                border-radius: 8px;
            }
            QTabBar::tab {
                background: #f1f5f9;
                color: #475569;
                padding: 8px 16px;
                border-top-left-radius: 6px;
                border-top-right-radius: 6px;
                margin-right: 2px;
                font-weight: 500;
            }
            QTabBar::tab:selected {
                background: #ffffff;
                color: #1e40af;
                font-weight: 600;
                border-top: 2px solid #1e40af;
            }
        )";
    }
};

#endif // THEMEHELPER_H
