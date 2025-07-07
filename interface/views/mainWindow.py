from PySide6.QtWidgets import QMainWindow, QLabel
from PySide6.QtGui import QPixmap
from PySide6.QtCore import Qt
import os

class MainWindow(QMainWindow):
    def __init__(self):
        super().__init__()
        self.setWindowTitle("Groundstation GAMA")
        self.resize(1000, 700)

        caminho_imagem = os.path.abspath("interface/resources/estrelado.png")  

        label = QLabel(self)
        pixmap = QPixmap(caminho_imagem)

        label.setPixmap(pixmap)
        label.setAlignment(Qt.AlignCenter)
        label.setScaledContents(True)  

        self.setCentralWidget(label)