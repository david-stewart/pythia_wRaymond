#include "MyInput.h"
#include "TFile.h"
#include "TH1F.h"

ClassImp(MyInput)
MyInput::MyInput(string argv) :
    InputBase{argv, true},
    nEvents{100},
    pthat_min{15.},
    pthat_max{40.},
    seed{-1}
{
    /* cout << "n_inputs " << n_inputs << endl; */
    if (give_help_msg) {
        cout << endl;
        cout << "------------------------------------------------------------------------------------------" << endl;
        cout << "inputs are: log-f-name root-f-name [nEvents=100] [pthatMin=" << pthat_min <<"] "
            <<  "[pthatMax="<<pthat_max<<"] [int seed=-1]" << endl;
        cout << "note: if seed == -1, then seed with be randomly set by the system time." << endl;
        cout << "------------------------------------------------------------------------------------------" << endl;
        exit (2);
    }

    if (n_inputs < 1) {
        cout << "fatal: bad inputs";
        cout << endl;
        cout << "------------------------------------------------------------------------------------------" << endl;
        cout << "inputs are: log-f-name root-f-name [nEvents=100] [pthatMin=" << pthat_min <<"] "
            <<  "[pthatMax="<<pthat_max<<"] [int seed=-1]" << endl;
        cout << "note: if seed == -1, then seed with be randomly set by the system time." << endl;
        cout << "------------------------------------------------------------------------------------------" << endl;
        exit(2);
    }

    string str;
    ss_args >> str;
    if (str.find(".root") == string::npos) {
        cout << "fatal, root file name must end in \".root\". Name given was\""<<str<<"\""<<endl;
        exit(1);
    }
    file = new TFile(str.c_str(),"RECREATE");

    if (n_inputs>1) ss_args >> nEvents;
    if (n_inputs>2) ss_args >> pthat_min;
    if (n_inputs>3) ss_args >> pthat_max;
    if (n_inputs>4) ss_args >> seed;

    if (seed == -1) {
      time_t a_time;
      time(&a_time);
      seed = (int)a_time;
      f_log << " # setting the seed from -1 to " << seed << " (from the clock)." << endl;
    }
        
    f_log << " # running with inputs: " << endl
          << " #    root file: " << str << endl
          << " #    nEvents:   " << nEvents << endl
          << " #    ptHatMin:  " << pthat_min << endl
          << " #    ptHatMax:  " << pthat_max << endl
          << " #    seed:      " << seed << endl;
}

MyInput::~MyInput() {
    file->Write();
    file->Close();
};

