# Notes Git — Apprentissage

## Les 3 zones de Git

| Zone | Description |
|------|-------------|
| Répertoire de travail | Tes fichiers sur le disque |
| Zone de transit (staging) | Fichiers prêts à committer |
| Dépôt local | Historique des commits |
| GitHub | Dépôt distant en ligne |

## Commandes essentielles

### Configuration initiale
git config --global user.name "TonNom"
git config --global user.email "ton@email.com"

### Commandes du quotidien
git status          → Voir les fichiers modifiés
git add fichier     → Ajouter un fichier au staging
git add .           → Ajouter TOUS les fichiers modifiés
git commit -m ""    → Créer un commit avec un message
git push            → Envoyer les commits vers GitHub
git pull            → Recevoir les changements de GitHub
git log --oneline   → Voir l'historique des commits
git diff            → Voir les modifications non commitées

### Commandes utiles
git clone URL       → Télécharger un dépôt GitHub
git branch          → Voir les branches
git checkout -b nom → Créer une nouvelle branche
git merge nom       → Fusionner une branche dans main

## Convention des messages de commit

type: description courte en minuscules

### Types de commits
feat     → Nouvelle fonctionnalité
fix      → Correction d'un bug
docs     → Documentation
test     → Code de test
refactor → Amélioration du code
chore    → Maintenance générale

### Exemples
feat: ajout lecture capteur BME280
fix: correction adresse I2C ENS160
docs: mise à jour README phase 2
refactor: optimisation lecture capteurs

## Bons vs mauvais commits

### Bons commits ✓
feat: ajout lecture température BME280
fix: correction bug affichage OLED
docs: ajout schéma câblage phase 2

### Mauvais commits ✗
modif
ça marche enfin
test123
update

## Erreurs fréquentes et solutions

### Erreur "rejected" lors d'un push
git pull
git push

### Annuler le dernier commit sans perdre les modifications
git reset --soft HEAD~1

### Restaurer un fichier à son état du dernier commit
git restore fichier.ino

## Concepts importants

### Branches
Copie indépendante du code pour expérimenter sans
affecter la version principale (main).

### Releases
Marqueur d'une version stable du projet.
Utilisé à la fin de chaque phase du projet.

### Fork
Copie complète d'un dépôt GitHub dans ton propre compte.
Utile pour contribuer à des projets open source.

### SSH
Méthode sécurisée pour connecter ton ordinateur à GitHub
sans taper ton mot de passe à chaque fois.