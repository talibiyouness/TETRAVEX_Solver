//Développeur : YOUNESS

#include <stdio.h>
#include <time.h>
#include <string.h>
#include <set>
using namespace std;

// La taille maximale d'un tableau donné est N * N
#define N 800

const int Gauche = 0;
const int Haut = 1;
const int Droite = 2;
const int Bas = 3;
const double temps_limit = 120;

// Classe de Carré
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



// Vérifiez que les deux carré, le premier et le deuxième peut être voisin
// le type est la direction du voisin

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

// Pour un plateau, nous indexons 0, 1, 2, ... (ligne * col - 1) 
// en continu pour chaque carré de plateau.

Carre carre[N * N];
bool visiter[N * N];
int ordre[N * N]; //Tableau qu’il contient la solution des indexes des i ème carré.
int ligne, col;
int child_id[N * N];//l'index du carré qui doit être vérifié pour déplacer le carré à la ième carré.
set<int> NonVisiter;//Un conteneur associatif  contient l'index de carré non visité
set<int>::iterator it;//itérateur qu’il va m’aider pour affecter le résultat de la recherche depuis le conteneur associatif.
clock_t temps_debut = clock();//Temps de début

// Vérifie la limite de temps actuelle temps_limit
bool VerificationTemps() {
    clock_t temps_fin = clock();
    if((double) (temps_fin - temps_debut) / CLOCKS_PER_SEC > temps_limit) {
        return false;
    }
    return true;
}

// Vérifier que carre [i] peut être déplacé vers le carré actuel
bool VerificationDeplacement(int index_recent, int i) {
    int ligne_recent = index_recent / col; // index de ligne du carré actuel
    int colonne_recent = index_recent % col; // index de colonne du carré actuel
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




//Trouver la solution 
bool TetravexBacktracking() {
    int pos_recent = 0;
    //Initialiser
    child_id[0] = 0;
    for(int i = 0; i <= ligne * col; i ++) NonVisiter.insert(i);
    while(true) {
        //Vérifier le temps
        if(!VerificationTemps()) {
            return false;
        }
        //Vérifier le déplacement de tout les carrés dans le plateau
        if(pos_recent == ligne * col) {
            return true;
        }
        
        // Trouver et définir l'id qui peut être possible move to pos_recent
        it = NonVisiter.lower_bound(child_id[pos_recent]);
        child_id[pos_recent] = *it;
        
        // Vérifier le déplacement impossible vers pos_recent
        if(child_id[pos_recent] == ligne * col) {
            pos_recent --;
            // Vérifier la boucle pour tous les statuts possibles
            if(pos_recent == -1) {
                return false;
            }
            NonVisiter.insert(ordre[pos_recent]);
            child_id[pos_recent] ++;
            continue;
        }
        // Vérification le déplacement de carré au plateau
        int id = child_id[pos_recent];
        if(!VerificationDeplacement(pos_recent, id)) {
            child_id[pos_recent] ++;
            continue;
        }
        // Déplacer l'id du carré vers la position du plateau
        NonVisiter.erase(id);
        ordre[pos_recent] = id;
        pos_recent ++;
        child_id[pos_recent] = 0;
    }
}


int main() {
    char FichierEntree[20];
    char FichierResultat[20];
    strcpy(FichierEntree, "random_3.txt");
    strcpy(FichierResultat, "3_Resultat.txt");
    freopen(FichierEntree, "r", stdin);

    int gauche, haut, droite, bas;

    scanf("%d%d", &ligne, &col);
    for(int i = 0; i < ligne; i ++) for(int j = 0; j < col; j ++) {
        scanf("%d%d%d%d", &gauche, &haut, &droite, &bas);
        Carre p(gauche, haut, droite, bas);
        carre[i * col + j] = p;
    }
//Trouver la solution 

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




