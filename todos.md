## Extraction de caractéristiques

[X] binarisation  
[X] extraction rectangle englobant : récupération des dimensions  
[X] normalisation imagette en carré de 128*128  
[X] zoning puis utilisation caractéristiques sur chaque zone :  
	[X] centre de masse  
	[X] densité (mean gray value) / ou proportion de pixels noirs  
[X] coins 

## Entraînement de classifiers
Etapes :  
kNN = bonne prise de température  
si les caractéristiques sont bonnes le kNN va bien marcher (car pas très performant globalement)  
-> MLP / arbre de décision (on peut dépasser les kNN si on les paramètre bien)  
-> SVM (encore mieux mais plus compliqué à mettre en œuvre)  
-> si possible, bagging / boosting

