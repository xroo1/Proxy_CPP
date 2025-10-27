CXX = g++
CXXFLAGS = -std=c++11 -pthread -Wall -O2 -Isrc
TARGET = myproxy
SRCDIR = src
OBJDIR = obj

SOURCES = $(wildcard $(SRCDIR)/*.cpp)
OBJECTS = $(SOURCES:$(SRCDIR)/%.cpp=$(OBJDIR)/%.o)

$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $(TARGET) $(CXXFLAGS)

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

install: $(TARGET)
	sudo mkdir -p /var/log/myproxy
	sudo mkdir -p /etc/myproxy
	sudo cp $(TARGET) /usr/local/bin/
	sudo cp config/myproxy.conf /etc/myproxy/
	sudo chmod +x /usr/local/bin/$(TARGET)

uninstall:
	sudo rm -f /usr/local/bin/$(TARGET)
	sudo rm -rf /etc/myproxy

clean:
	rm -rf $(OBJDIR) $(TARGET) logs

.PHONY: install uninstall clean
