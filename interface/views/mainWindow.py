from PySide6.QtWidgets import QMainWindow, QWidget, QLabel, QVBoxLayout
from PySide6.QtGui import QPixmap
from PySide6.QtCore import Qt
import os

class MainWindow(QMainWindow):
    def __init__(self):
        super().__init__()
        self.setWindowTitle("Groundstation GAMA")
        self.resize(800, 600)  

        # Container central
        central_widget = QWidget(self)
        self.setCentralWidget(central_widget)

        # Layout principal
        layout = QVBoxLayout(central_widget)
        layout.setContentsMargins(0, 0, 0, 0)

        # Caminhos das imagens
        caminho_fundo = os.path.abspath("interface/resources/estrelado.png")
        caminho_icone = os.path.abspath("interface/resources/gamaY.png")

        # Imagem de fundo
        label_fundo = QLabel()
        pixmap_fundo = QPixmap(caminho_fundo)
        label_fundo.setPixmap(pixmap_fundo)
        label_fundo.setScaledContents(True) 
        layout.addWidget(label_fundo)  

        # icone gama
        label_icone = QLabel(central_widget)
        pixmap_icone = QPixmap(caminho_icone)
        label_icone.setPixmap(pixmap_icone)
        label_icone.setScaledContents(True)
        label_icone.resize(80, 80)
        label_icone.move(20, 20)  
