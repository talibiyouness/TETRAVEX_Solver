

#include <stdio.h>
#include <time.h>
#include <string.h>
#include <set>
using namespace std;

#define N 800

const int Gauche = 0;
const int Haut = 1;
const int Droite = 2;
const int Bas = 3;

const double limit_time = 120;
class Carre {
public:
    int gauche;
    int haut;
    int droite;
    int bas;
    Carre(int gauche = 0, int haut = 0, int droite = 0, int bas = 0) {
        this->gauche = gauche;
        this->haut = haut;
        this->droite = droite;
        this->bas = bas;
    }

    void Saisir() {
        printf("%d %d %d %d\n", gauche, haut, droite, bas);
    }

};



bool VerificationPlateau(Carre premier, Carre deuxieme, int type) {
    if(type == Gauche) {
        if(premier.droite == deuxieme.gauche) return true;
        return false;
    }
    if(type == Droite) {
        if(premier.gauche == deuxieme.droite) return true;
        return false;
    }
    if(type == Haut) {
        if(premier.bas == deuxieme.haut) return true;
        return false;
    }
    if(premier.haut == deuxieme.bas) return true;
    return false;
}

Carre carre[N * N];
bool visiter[N * N];
int ordre[N * N];
int ligne, col;
int max_val = 0;
int child_id[N * N];
set<int> NonVisiter;
set<int>::iterator it;
clock_t temps_debut = clock();

bool VerificationTemps() {
    clock_t temps_fin = clock();
    if((double) (temps_fin - temps_debut) / CLOCKS_PER_SEC > limit_time) {
        return false;
    }
    return true;
}

bool VerificationDeplacement(int index_recent, int i) {
    int ligne_recent = index_recent / col;
    int colonne_recent = index_recent % col;
    bool deplacer = true;
    if(ligne_recent != 0) {
        Carre haut_Tetravex = carre[ordre[index_recent - col]];
        if(VerificationPlateau(haut_Tetravex, carre[i], Haut) == false) {
            deplacer = false;
        }
    }
    if(colonne_recent != 0) {
        Carre gauche_Tetravex = carre[ordre[index_recent - 1]];
        if(VerificationPlateau(gauche_Tetravex, carre[i], Gauche) == false) {
            deplacer = false;
        }
    }
    return deplacer;
}




bool TetravexBacktracking() {
    int pos_recent = 0;
    child_id[0] = 0;
    for(int i = 0; i <= ligne * col; i ++) NonVisiter.insert(i);
    while(true) {
        if(!VerificationTemps()) {
            return false;
        }
        if(pos_recent == ligne * col) {
            return true;
        }

        it = NonVisiter.lower_bound(child_id[pos_recent]);
        child_id[pos_recent] = *it;

        if(child_id[pos_recent] == ligne * col) {
            pos_recent --;
            if(pos_recent == -1) {
                return false;
            }
            NonVisiter.insert(ordre[pos_recent]);
            child_id[pos_recent] ++;
            continue;
        }
        int id = child_id[pos_recent];
        if(!VerificationDeplacement(pos_recent, id)) {
            child_id[pos_recent] ++;
            continue;
        }

        NonVisiter.erase(id);
        ordre[pos_recent] = id;
        pos_recent ++;
        child_id[pos_recent] = 0;
    }
}


int main() {
    char FichierEntree[20];
    char FichierResultat[20];
    strcpy(FichierEntree, "random_500.txt");
    strcpy(FichierResultat, "random_500_Resultat.txt");
    freopen(FichierEntree, "r", stdin);

    int gauche, haut, droite, bas;

    scanf("%d%d", &ligne, &col);
    for(int i = 0; i < ligne; i ++) for(int j = 0; j < col; j ++) {
        scanf("%d%d%d%d", &gauche, &haut, &droite, &bas);
        Carre p(gauche, haut, droite, bas);
        carre[i * col + j] = p;
    }
    int cnt = 0;
    if(TetravexBacktracking()) {
        printf("***** TETRAVEX est bien été résolut *****");
        freopen(FichierResultat, "w", stdout);
        printf("%d %d\n", ligne, col);
        for(int i = 0; i < ligne * col; i ++) {
            carre[ordre[i]].Saisir();
        }

    } else {
        printf("***** Il y a pas de solution *****");
    }

}




