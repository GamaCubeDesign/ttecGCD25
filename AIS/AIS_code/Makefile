# Compilador e flags
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2

# Bibliotecas necessárias (WiringPi para serial)
LIBS = -lwiringPi

# Inclua aqui todos os arquivos .cpp do projeto
SOURCES = AIS.cpp Receiver.cpp Decodificação.cpp 
OBJECTS = $(SOURCES:.cpp=.o)

TARGET = ais_receiver

# Regra principal
all: $(TARGET)

# Linkagem final
$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $(TARGET) $(LIBS)

# Compilação de cada .cpp para .o
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Limpeza
clean:
	rm -f $(OBJECTS) $(TARGET)

# Limpeza total + arquivos auxiliares
distclean: clean
	rm -f *.json *.log
