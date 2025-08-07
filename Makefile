#Team Names: Michael Corn and Simmon Quan

#Edoras Usernames: cssc1415 and cssc1450

#Class Info: CS 480 Operating Systems, Summer Session 2 2025

#Assignment #2: Experimental Shell "XSH"

#File Name: Makefile
	

CXX = g++
CXXFLAGS = -Wall -std=c++11 
TARGET = sim

all: $(TARGET)

$(TARGET): sim.cpp
	$(CXX) $(CXXFLAGS) -o $(TARGET) sim.cpp

clean:
	rm -f $(TARGET) 
