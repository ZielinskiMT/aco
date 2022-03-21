#include "argprs.h"

#include <vector>
#include <string_view>
#include <stdexcept>


ArgPrs::ArgPrs(int argc, char** argv):
m_Alfa(1.0),
m_Beta(1.0),
m_Q(1.0),
m_p(0.2),
m_antNo(100),
m_MaxNoChangeIterations(5),
m_SpNoChangeIterations(2),
m_arrOptUsed(),
m_args(argv + 1, argv + argc)
{}

double ArgPrs::checkDoublekArg(int & k, bool & optUsed){

    int argc = m_args.size();

    if( optUsed == true )
        throw std::runtime_error( "argument " + m_args[k] + " can not be used more than once!");
    if((k+1) < argc){
        optUsed = true;
        k++;
        return std::stod(m_args[k]);
    }
    else
        throw std::runtime_error( "argument " + m_args[k] +  " have to be followed by value.");
}

unsigned int ArgPrs::checkIntkArg(int & k, bool & optUsed){

    int argc = m_args.size();

    if( optUsed == true )
        throw std::runtime_error( "argument " + m_args[k] + " can not be used more than once!");
    if((k+1) < argc){
        optUsed = true;
        k++;
        return static_cast<unsigned int>(std::stoi(m_args[k]));
    }
    else
        throw std::runtime_error( "argument " + m_args[k] +  " have to be followed by value.");
}

void ArgPrs::parse(){

    int argc = m_args.size();

    if (argc > 64) { 
        throw std::runtime_error("too many input parameters!"); 
    }

    for(int k = 0; k < argc; k++){
        if((m_args[k] == "-a")||(m_args[k] == "--alpha")){
            m_Alfa = checkDoublekArg(k, m_arrOptUsed[0]);
        }
        else if((m_args[k] == "-b")||(m_args[k] == "--beta")){
            m_Beta = checkDoublekArg(k, m_arrOptUsed[1]);
        }
        else if((m_args[k] == "-Q")||((m_args[k] == "--pehromone-const"))){
            m_Q = checkDoublekArg(k, m_arrOptUsed[2]);
        }
        else if((m_args[k] == "-r")||((m_args[k] == "--evaporation-const"))){
            m_p = checkDoublekArg(k, m_arrOptUsed[3]);
        }
        else if((m_args[k] == "-s")||((m_args[k] == "--no-change-iterations-sp"))){
            m_SpNoChangeIterations =checkIntkArg(k, m_arrOptUsed[4]);
        }
        else if((m_args[k] == "-m")||((m_args[k] == "--max-no-change-iterations"))){
            m_MaxNoChangeIterations =checkIntkArg(k, m_arrOptUsed[5]);
        }
        else if((m_args[k] == "-n")||((m_args[k] == "--number-of-ants"))){
            m_antNo =checkIntkArg(k, m_arrOptUsed[6]);
        }
        else{
            throw std::runtime_error("Invalid command line arguments");
        }
    }
}