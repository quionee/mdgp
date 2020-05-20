#include <iostream>
#include <vector>
#include <fstream>

using namespace std;

const string NUM_MAX[3] = {"3", "4", "5"};
const string INI_SOL[3] = {"lh", "gc", "wj"};
const string LS[4] = {"insertion", "swap", "3_chain", " "};
const string LS_IMP[4] = {"insertion_imp", "swap_imp", "3_chain_imp", " "};
ofstream file("possibilidades", ios::trunc);

void insertion_strong(vector<string> &solutionSequence) {
    for (int i = 1; i < 4; ++i) {
        solutionSequence.push_back(LS[i] + " )");
        for (int j = 0; j < solutionSequence.size(); ++j) {
            file << solutionSequence[j] << " ";
        }
        file << endl;
        
        solutionSequence.pop_back();
    }
}

void swap_strong(vector<string> &solutionSequence) {
    for (int i = 0; i < 3; i += 2) {
        solutionSequence.push_back(LS[i] + " )");
        for (unsigned int j = 0; j < solutionSequence.size(); ++j) {
            file << solutionSequence[j] << " ";
        }
        file << endl;

        solutionSequence.pop_back();
    }

    solutionSequence.push_back(LS[3] + " )");
    for (unsigned int j = 0; j < solutionSequence.size(); ++j) {
        file << solutionSequence[j] << " ";
    }
    file << endl;

    solutionSequence.pop_back();
}

void three_chain_strong(vector<string> &solutionSequence) {
    for (int i = 0; i < 2; ++i) {
        solutionSequence.push_back(LS[i] + " )");
        for (unsigned int j = 0; j < solutionSequence.size(); ++j) {
            file << solutionSequence[j] << " ";
        }
        file << endl;

        solutionSequence.pop_back();
    }

    solutionSequence.push_back(LS[3] + " )");
    for (unsigned int j = 0; j < solutionSequence.size(); ++j) {
        file << solutionSequence[j] << " ";
    }
    file << endl;

    solutionSequence.pop_back();
}


void localSearchStrong(vector<string> &solutionSequence) {
    for (int j = 0; j < 3; ++j) {
        solutionSequence.push_back(LS[j]);
        if (LS[j] == "insertion") {
            insertion_strong(solutionSequence);
        }
        else if (LS[j] == "swap") {
            swap_strong(solutionSequence);
        }
        else {   // LS[j] == 3_chain
            three_chain_strong(solutionSequence);
        }

        solutionSequence.pop_back();
    }
}

void insertion_imp(vector<string> &solutionSequence) {
    for (int i = 1; i < 4; ++i) {
        solutionSequence.push_back(LS_IMP[i] + " ),");
        
        localSearchStrong(solutionSequence);
        
        solutionSequence.pop_back();
    }
}

void swap_imp(vector<string> &solutionSequence) {
    for (int i = 0; i < 3; i += 2) {
        solutionSequence.push_back(LS_IMP[i] + " ),");
        
        localSearchStrong(solutionSequence);

        solutionSequence.pop_back();
    }

    solutionSequence.push_back(LS_IMP[3] + " ),");
    
    localSearchStrong(solutionSequence);

    solutionSequence.pop_back();
}

void three_chain_imp(vector<string> &solutionSequence) {
    for (int i = 0; i < 2; ++i) {
        solutionSequence.push_back(LS_IMP[i] + " ),");

        localSearchStrong(solutionSequence);

        solutionSequence.pop_back();
    }

    solutionSequence.push_back(LS_IMP[3] + " ),");

    localSearchStrong(solutionSequence);

    solutionSequence.pop_back();
}

void localSearchImpWeak(vector<string> &solutionSequence) {
    for (int j = 0; j < 3; ++j) {
        solutionSequence.push_back(LS_IMP[j]);
        if (LS_IMP[j] == "insertion_imp") {
            insertion_imp(solutionSequence);
        }
        else if (LS_IMP[j] == "swap_imp") {
            swap_imp(solutionSequence);
        }
        else {   // LS_IMP[j] == 3_chain
            three_chain_imp(solutionSequence);
        }

        solutionSequence.pop_back();
    }
}

void insertion(vector<string> &solutionSequence) {
    for (int i = 1; i < 4; ++i) {
        solutionSequence.push_back(LS[i] + ",");

        localSearchImpWeak(solutionSequence);

        solutionSequence.pop_back();
    }
}

void swap(vector<string> &solutionSequence) {
    for (int i = 0; i < 3; i += 2) {
        solutionSequence.push_back(LS[i] + ",");

        localSearchImpWeak(solutionSequence);

        solutionSequence.pop_back();
    }

    solutionSequence.push_back(LS[3] + ",");

    localSearchImpWeak(solutionSequence);

    solutionSequence.pop_back();
}

void three_chain(vector<string> &solutionSequence) {
    for (int i = 0; i < 2; ++i) {
        solutionSequence.push_back(LS[i] + ",");
        
        localSearchImpWeak(solutionSequence);
        
        solutionSequence.pop_back();
    }

    solutionSequence.push_back(LS[3] + ",");
    
    localSearchImpWeak(solutionSequence);

    solutionSequence.pop_back();
}

void weakPerturbation(vector<string> &solutionSequence) {
    for (int i = 0; i < 3; ++i) {
        solutionSequence.push_back(LS[i]);
        if (LS[i] == "insertion") {
            insertion(solutionSequence);
        }
        else if (LS[i] == "swap") {
            swap(solutionSequence);
        }
        else {   // LS[i] == 3_chain
            three_chain(solutionSequence);
        }
        solutionSequence.pop_back();
    }
}

void maxSearch(vector<string> &solutionSequence) {
    solutionSequence.push_back("maxSearch(");
    for (int i = 0; i < 3; ++i) {
        solutionSequence.push_back(NUM_MAX[i] + ",");
        weakPerturbation(solutionSequence);
        solutionSequence.pop_back();
    }
}

void ims() {
    vector<string> solutionSequence;
    for (int i = 0; i < 3; ++i) {
        solutionSequence.push_back("ims(");
        solutionSequence.push_back(INI_SOL[i] + ",");
        maxSearch(solutionSequence);
        solutionSequence.clear();
    }
}

int main() {
    ims();
}