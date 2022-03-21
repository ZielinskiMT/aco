#ifndef ARGPRS_H
#define ARGPRS_H

#include <vector>
#include <string>

class ArgPrs{
    private:
        const std::vector<std::string> m_args;

        double m_Q;
        double m_p;
        double m_Alfa;
        double m_Beta;
        unsigned int m_antNo;
        unsigned int m_MaxNoChangeIterations;
        unsigned int m_SpNoChangeIterations;

        bool m_arrOptUsed[7];

        double checkDoublekArg(int & k, bool & optUsed);
        unsigned int checkIntkArg(int & k, bool & optUsed);

    public:
        ArgPrs(int argc, char** argv);


        void parse();
        const double Q() const { return m_Q; }
        const double p() const { return m_p; }
        const double Alfa() const { return m_Alfa;}
        const double Beta() const { return m_Beta; }
        const unsigned int AntNo() const { return m_antNo; }
        const unsigned int MaxNoChangeIterations() const { return m_MaxNoChangeIterations; }
        const unsigned int SpNoChangeIterations() const { return m_SpNoChangeIterations; }
        const std::string UsageInfo() { 
            return "Usage: " + std::string(m_args[0]) + " <option(s)>\n" +
                "Options:\n" +
                "\t-h,--help                        Show this help message\n" +
                "\t-a,--alpha                       Alpha exponent\n" +
                "\t-b,--beta                        Beta exponent\n" +
                "\t-Q,--pehromone-const             Pheromone constant\n" +
                "\t-p,--evaporation-const           Pheromone evaporation constant\n" +
                "\t-n,--number-of-ants              Number of ants to be used for calculations\n" +
                "\t-s,--no-change-iterations-sp     Number of iterations, with no best path change, to randomly distribute ants\n" +
                "\t-m,--max-no-change-iterations    Number of iterations, with no best path change, to end calculations\n";
        }

};

#endif /* ARGPRS_H */
