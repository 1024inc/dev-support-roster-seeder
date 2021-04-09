#include <iostream>
#include <stdlib.h>
#include <string>
#include <stddef.h>
#include <vector>
#include <fstream>
#include <ctime>

using namespace std;

class parser
{
    private:
        virtual void abstract() = 0;
    
    public:
        static void parse(string rosterEUFile, string rosterUSFile, string scheduleFile, int nbDays)
        {
            vector<string> roster_us;
            vector<string> roster_eu;
            ifstream sourceEU(rosterEUFile);
            ifstream sourceUS(rosterUSFile);
            ofstream destination(scheduleFile, ofstream::trunc);
            string name;
            tm* day;
            time_t clock;
            int cnt, cntRoster;

            if (!sourceEU.is_open())
            {
                throw "Can't open " + rosterEUFile;
            }

            if (!sourceUS.is_open())
            {
                throw "Can't open " + rosterUSFile;
            }

            if (!destination.is_open())
            {
                throw "Can't open " + scheduleFile;
            }

            while (sourceUS >> name)
            {
                roster_us.push_back(name);
            }

            while (sourceEU >> name)
            {
                roster_eu.push_back(name);
            }

            clock = time(0);
            day = localtime(&clock);
            cntRoster = 0;
            for (cnt = 0; cnt < nbDays; cnt++)
            {
                if (day->tm_wday != 0 && day->tm_wday != 6)
                {
                    destination << "\"" + to_string(day->tm_mon + 1) + "/" + to_string(day->tm_mday) + "/" + to_string(day->tm_year + 1900) +  "\",\"" + roster_eu[cntRoster % roster_eu.size()] + "\",\"" + roster_us[cntRoster % roster_us.size()] + "\"\n";
                }
                else if (day->tm_wday == 6)
                {
                    cntRoster++;
                }
                day->tm_mday += 1;
                mktime(day);
            }

            return;
        }
};

int main(int argc, char const *argv[])
{
    if (argc != 5)
    {
        cout << "command line params expected: roster_eu_file roster_us_file, destination_file, nb_of_days\n";
        return 0;
    }

    parser::parse(argv[1], argv[2], argv[3], atoi(argv[4]));
}