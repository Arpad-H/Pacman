# 3D Pac Man - OpenGL Spiel

## Beschreibung
Das Projekt ist eine 3D-Implementierung des klassischen Spiels Pac-Man, entwickelt mit OpenGL. Im Gegensatz zum traditionellen Pac-Man-Spiel, das auf einer einzigen Ebene stattfindet, bietet dieses Spiel ein einzigartiges Erlebnis in einem dreidimensionalen Raum. Das Level ist als Würfel gestaltet, wobei jede Seite (oder "Face") des Würfels ein eigenes Labyrinth (Maze) enthält. Die Labyrinthe werden zur Compile-Zeit generiert und sind daher bei jedem Spiel neu, was für zusätzliche Herausforderungen und Abwechslung sorgt.

Spieler steuern Pac-Man mit den W, A, S, D Tasten durch die Labyrinthe, sammeln Pellets und erhöhen so ihr Score. 
Doch Vorsicht ist geboten: Wird Pac-Man von einem der Geister erwischt, ist das Spiel vorbei. Die Geister nutzen einen Pathfinding-Algorithmus und bewegen sich mit einem charakteristischen "Retro" Blink-Effekt.

## Features
- **Dynamische Labyrinth-Generierung:** Jedes Spiel bietet ein neues Labyrinth, basierend auf dem DFS-Algorithmus.
- **Intelligente Geister:** Geister verfolgen Pac-Man mit einem BFS-basierten Pathfinding-Algorithmus.
- **3D-Grafik und Effekte:** Neben der 3D-Darstellung des Labyrinths beinhaltet das Spiel eine custom Galaxy Skybox und Retro Post-Processing-Effekte.
- **Steuerung:** Die Bewegung von Pac-Man wird mit den Tasten W, A, S, D gesteuert.

## Verwendung
Starte das Spiel mit dee `.exe` Datei . Steuere Pac-Man durch die Labyrinthe auf den verschiedenen Seiten des Würfels, sammle Pellets und meide die Geister. Ziel ist es, so viele Punkte wie möglich zu sammeln, bevor eines der Geister dich erwischt.

## Code-Highlights - 
1. **Labyrinth-Generierung (`Maze.cpp`):** Das Labyrinth basiert auf einem Depth-First Search (DFS) Algorithmus, angelehnt an die in diesem Research Paper dargestellten Regeln: [DFS Labyrinth Generation](https://people.eecs.berkeley.edu/~sequin/CS285/PROJ_06/Adarsh--Athulan/AA_report/report.html). Es ist wichtig zu beachten, dass der Code eigenständig entwickelt wurde und keine direkte Übernahme stattgefunden hat.
2. **Pathfinding der Geister (`Ghost.cpp/findPath`):** Die Geister nutzen einen Breadth-First Search (BFS) Algorithmus, um den kürzesten Weg zu Pac-Man zu finden.
3. **Sampling für Wände:** Die Applikation verwendet Sampling-Techniken, um die Wände des Labyrinths zu erstellen, was für eine effiziente Darstellung und Interaktion sorgt.
4. **Galaxy Skybox:** Das Spiel verfügt über eine custom Galaxy Skybox, die das 3D-Erlebnis weiter vertieft.
5. **Retro Effekte:** Für die visuelle Gestaltung werden Post-Processing-Effekte im Retro-Stil verwendet, die dem Spiel ein einzigartiges Aussehen und Gefühl verleihen.
