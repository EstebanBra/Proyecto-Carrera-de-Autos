  GNU nano 7.2                                                                                        Makefile                                                                                                 
# Nombre del compilador
CXX = g++

# Opciones de compilaci  n (puedes agregar -g para depuraci  n o -O2 para optimizaci  n)
CXXFLAGS = -Wall -std=c++11

# Nombre del ejecutable
TARGET = carrera_autos

# Archivos fuente
SRCS = CarreraDeAutos.cpp

# Archivo objeto (intermedio)
OBJS = $(SRCS:.cpp=.o)

# Regla principal: compilar el ejecutable
$(TARGET): $(OBJS)
        $(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

# Regla para compilar los archivos fuente (.cpp) a archivos objeto (.o)
%.o: %.cpp
        $(CXX) $(CXXFLAGS) -c $< -o $@

# Regla para limpiar los archivos generados
clean:
        rm -f $(OBJS) $(TARGET)

