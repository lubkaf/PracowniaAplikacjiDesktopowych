# This Python file uses the following encoding: utf-8
import sys
import math

from PySide6.QtWidgets import QApplication, QWidget
from math_operations import MathOperations

# Important:
# You need to run the following command to generate the ui_form.py file
#     pyside6-uic form.ui -o ui_form.py, or
#     pyside2-uic form.ui -o ui_form.py
from ui_form import Ui_Widget

class Widget(QWidget):
    def __init__(self, parent=None):
        super().__init__(parent)
        self.ui = Ui_Widget()
        self.ui.setupUi(self)

        self.calc = MathOperations()

        self.ui.btnAdd.clicked.connect(self.handle_add)
        self.ui.btnSubstract.clicked.connect(self.handle_substract)
        self.ui.btnMultiply.clicked.connect(self.handle_multiply)
        self.ui.btnDivide.clicked.connect(self.handle_divide)

    def get_values(self):
        try:
            a = float(self.ui.txtFirstNum.text())
            b = float(self.ui.txtSecNum.text())
            return a, b
        except ValueError:
            self.ui.txtResult.setText("Błąd: Wpisz liczby!")
            return None, None

    def handle_add(self):
        a, b = self.get_values()
        if a is not None:
            res = self.calc.Add(a,b)
            self.ui.txtResult.setText(str(res))


    def handle_substract(self):
        a, b = self.get_values()
        if a is not None:
            res = self.calc.Substract(a,b)
            self.ui.txtResult.setText(str(res))

    def handle_multiply(self):
        a, b = self.get_values()
        if a is not None:
            res = self.calc.Multiply(a,b)
            self.ui.txtResult.setText(str(res))

    def handle_divide(self):
        a, b = self.get_values();

        if a is not None:
            res = self.calc.Divide(a,b)
            if math.isnan(res):
                self.ui.txtResult.setText("Nie dziel przez 0")
            else:
                self.ui.txtResult.setText(str(res))
if __name__ == "__main__":
    app = QApplication(sys.argv)
    widget = Widget()
    widget.show()
    sys.exit(app.exec())
