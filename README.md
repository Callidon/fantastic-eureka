# fantastic-eureka
Application de messagerie instantanée entre machines distantes

Installation
===============
Les dépendances pour compiler et utiliser notre application sont les suivantes :
* **gcc** en version 4.8.4 ou supérieure
* **make** en version 3.81 ou supérieure
* **ncurses** en version 5.9 ou supérieure

Pour récupérer le code source, vous pouvez le télécharger à l'adresse [suivante](https://github.com/Callidon/fantastic-eureka), ou alors en utilisant **git** avec la commande suivante :
```bash
git clone https://github.com/Callidon/fantastic-eureka.git
```

Pour compiler le programme, exécutez la commande suivante dans le répertoire du projet :
```bash
# pour compiler le client
make client

# pour compiler le serveur
make server
```

Pour lancer le serveur, exécutez la commande suivante, en passant le mot de passe que vous désirez attribuer au serveur en paramètre
```bash
./server <mot-de-passe>
```

Pour lancer un client, exécutez la commande suivante, en passant l'adresse du serveur auquel vous voulez vous connecter en paramètre :
```bash
./client <adresse>
```

