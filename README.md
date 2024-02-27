# 3D Pac Man - OpenGL Spiel

## Beschreibung
Das Projekt ist eine 3D-Implementierung des klassischen Spiels Pac-Man, entwickelt mit OpenGL. Im Gegensatz zum traditionellen Pac-Man-Spiel, das auf einer einzigen Ebene stattfindet, bietet dieses Spiel ein einzigartiges Erlebnis in einem dreidimensionalen Raum. Das Level ist als Würfel gestaltet, wobei jede Seite (oder "Face") des Würfels ein eigenes Labyrinth (Maze) enthält. Die Labyrinthe werden zur Compile-Zeit generiert und sind daher bei jedem Spiel neu, was für zusätzliche Herausforderungen und Abwechslung sorgt.

Spieler steuern Pac-Man mit den W, A, S, D Tasten durch die Labyrinthe, sammeln Pellets und erhöhen so ihr Score. 
Doch Vorsicht ist geboten: Wird Pac-Man von einem der Geister erwischt, ist das Spiel vorbei. Die Geister nutzen einen Pathfinding-Algorithmus und bewegen sich mit einem charakteristischen "Retro" Blink-Effekt.

## Steuerung
Die Bewegung von Pac-Man wird mit den Tasten W, A, S, D gesteuert.

## Features
- **Dynamische Labyrinth-Generierung:** Jedes Spiel bietet ein neues Labyrinth, basierend auf dem DFS-Algorithmus.
- **Intelligente Geister:** Geister verfolgen Pac-Man mit einem BFS-basierten Pathfinding-Algorithmus.
- **3D-Grafik und Effekte:** Neben der 3D-Darstellung des Labyrinths beinhaltet das Spiel eine custom Galaxy Skybox und Retro Post-Processing-Effekte.

## Verwendung
Starten Sie das Spiel mit der `.exe` Datei . Steuern Sie Pac-Man durch die Labyrinthe auf den verschiedenen Seiten des Würfels, sammeln Sie Pellets und meiden Sie die Geister. Ziel ist es, so viele Punkte wie möglich zu sammeln, bevor eines der Geister Sie erwischt.

## Code-Highlights - Geht über die Vorlesung hinaus
1. **Labyrinth-Generierung (`Maze.cpp`):** Das Labyrinth basiert auf einem Depth-First Search (DFS) Algorithmus, angelehnt an die in diesem Research Paper dargestellten Regeln: [DFS Labyrinth Generation](https://people.eecs.berkeley.edu/~sequin/CS285/PROJ_06/Adarsh--Athulan/AA_report/report.html). Es ist wichtig zu beachten, dass der Code eigenständig entwickelt wurde und keine direkte Übernahme stattgefunden hat.
2. **Pathfinding der Geister (`Ghost.cpp/findPath`):** Die Geister nutzen einen Breadth-First Search (BFS) Algorithmus, um den kürzesten Weg zu Pac-Man zu finden.
3. **Instancing für Wände und Pellets:** Die Applikation verwendet Instancing-Techniken. Hierzu benutzen wir SSBO's um die Instanzdaten zu übergeben. Es wird jeweils nurt ein einziger Drawcall gemacht statt den ~6000. Wir habe nauch die Vertexbuffer klasse angepasst damit diese Instancing unterstützt. Allerdings hat dies nicht ganz so geklappt wie erhoft weswegen wir auf die SSBO's umgestiegen sind. RenderDoc debugging hat ergeben das nach instanz 23 keine Daten mehr ankomme. Bis dahin wird alles dargestellt wie gewünscht allerding dannach nicht mehr. 
4. **Galaxy Skybox:** Anstatt eines einfachen Würfels als Himmel, verfügt das Spiel über eine custom  Skybox Klasse mit opengls cubemap textures. Diese Skybox können wir auf die Objekte im level mapen (wie bei den Wänden, welche spiegle darstellen sollen) um Reflektionen der Umgebung zu simulieren. wir haben die Klasse unter zuhilfenahme von learn opengl https://learnopengl.com/Advanced-OpenGL/Cubemaps erarbeitet. 
5. **Retro Effekte:** Für die visuelle Gestaltung werden Post-Processing-Effekte im Retro-Stil verwendet, die dem Spiel ein einzigartiges Aussehen und Gefühl verleihen. Wir haben die Render Pipeline um postprocessing erweitert. Im ersten Pass wird die Szene Gerendert, beim zwieten wird Imgui in einen custom Framebuffer gerendert. Diese werden mit einem CRT effekt versehen. Shaderfunktion basierend auf dem Video https://www.youtube.com/watch?v=aWdySZ0BtJs
