# VirtualPolyhedraRedactor
Potentially simple interactive qt-based redactor for virtual polyhedra diagrams

## What is virtual polyhedra?
Virtual polyhedron is a 2-dimensioal CW-complex of special type. Such an objects can define topological 3-manifold. This representation is convinient to explore the Matveev complexity of 3-manifolds. 

Formally talking, virtual polyhedron is a CW-complex with only 4 allowed neigborhoods for each point:

![neighborhood](https://user-images.githubusercontent.com/61385404/186932269-30cb0426-9883-4e91-aa62-c90e8d1933db.jpg)

For a sake of simplicity one could think of virtual polyhedra as 4-regular non-oriented graphs with a set of disks glued to this graph such that to every edge of a graph locally glued 3 disks:

![Screenshot from 2021-06-02 13-11-00](https://user-images.githubusercontent.com/61385404/186931743-56f50938-7720-48af-830b-6e087f11d4e5.png)

## What do this project do?
To visualize theese objects special diagrams are used: 

![WeidPoly](https://user-images.githubusercontent.com/61385404/186933841-9da977e5-8ae3-49bd-8b1b-7cd13becc1c6.png)

But nowdays it takes about an hour to draw such a diagram. This project is aimed to speed up this process. It is just a home-project so development is a bit slow. But such an example now can be done in a minute approximately:

![VPR-0](https://user-images.githubusercontent.com/61385404/186934126-9985e4ce-97cb-4177-b09f-bc5549e1c7e8.png)

## How does it work?

On the current state the only thing you can do is to draw a whatever graph you want. To connect two nodes with names "x" and "y" by edge you need to write the pair "(x,y)" into input line. You can pass several pairs into input line divided by spaces. If there is no node with such a name it will be automatically added to the screen. Whenever you want -- you can add more edges or nodes. 

You can move nodes by a cursor and change curvature of edges by dragging the middle of the arc. By right-click on the middle you can open a menu with all possible rotations of an edge.

If the edge turns gray then you placed the edge too close to another one (bad crossing), try to move it in another position. It was done to hide weird effects of such an intersection and should not bother correct layouts of diagram (where all important parts of diagram can be seen).

Nevertheless,
![](ItWorks.png)
*\*somehow\**
