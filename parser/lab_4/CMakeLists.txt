cmake_minimum_required(VERSION 3.17)
project(lab_4)

set(CMAKE_CXX_STANDARD 17)

add_executable(lab_4 main.cpp
        TemplateTuplePrinter.h
        TemplateTupleReader.h
        CSVTupleParser.h
        CSVParserException.h
        CSVParserArgsHandler.h)
