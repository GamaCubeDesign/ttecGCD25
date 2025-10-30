# Compilador e opções básicas
CXX = g++
CXXFLAGS = -Wall -O2 -std=c++17

# Nome do executável
TARGET = lora_app

# Fontes e objetos
SRC = main.cpp Integration.cpp Moden.cpp Module.cpp
OBJ = $(SRC:.cpp=.o)

# Regra padrão
all: $(TARGET)

# Linkagem final
$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Compilação de cada arquivo .cpp
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Limpeza
clean:
	rm -f $(OBJ) $(TARGET)

.PHONY: all clean
