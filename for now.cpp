#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
using namespace std;

struct Node {
    char id;
    int weight;
    Node* next;

    Node(char i, int w, Node* n) {
        id = i;
        weight = w;
        next = n;
    }
};

struct Vertex {
    char id;
    Node* adj_list;
    Vertex* next;

    Vertex(char i, Node* a, Vertex* n) {
        id = i;
        adj_list = a;
        next = n;
    }
};

struct Graph {
    Vertex* vertices;

    Graph() {
        vertices = nullptr;
    }

    void addVertex(char id) {
        if (!vertex_exists(id)) {
            Vertex* new_vertex = new Vertex(id, nullptr, vertices);
            vertices = new_vertex;
        }
    }

    bool vertex_exists(char id) {
        Vertex* temp = vertices;
        while (temp) {
            if (temp->id == id)
                return true;
            temp = temp->next;
        }
        return false;
    }

    void add_edge(char src, char dest, int weight) {
        addVertex(src);
        addVertex(dest);

        Vertex* srcVertex = find_vertex(src);
        if (srcVertex) {
            Node* new_node = new Node(dest, weight, srcVertex->adj_list);
            srcVertex->adj_list = new_node;
        }
    }

    Vertex* find_vertex(char id) {
        Vertex* temp = vertices;
        while (temp) {
            if (temp->id == id) {
                return temp;
            }
            temp = temp->next;
        }
        return nullptr;
    }

    void print_graph() {
        Vertex* temp = vertices;
        while (temp) {
            cout << "Vertex " << temp->id << ":";
            Node* adj = temp->adj_list;
            while (adj) {
                cout << " -> " << adj->id << "(" << adj->weight << ")";
                adj = adj->next;
            }
            cout << endl;
            temp = temp->next;
        }
    }

    ~Graph() {
        Vertex* temp = vertices;
        while (temp) {
            Node* adj = temp->adj_list;
            while (adj) {
                Node* toDelete = adj;
                adj = adj->next;
                delete toDelete;
            }
            Vertex* toDelete = temp;
            temp = temp->next;
            delete toDelete;
        }
    }
};

void read_file(Graph& g, const string& filename) {
    ifstream file(filename);
    string line, intersection1, intersection2;
    int travelTime;

    getline(file, line);

    while (getline(file, line)) {
        stringstream ss(line);
        getline(ss, intersection1, ',');
        getline(ss, intersection2, ',');
        ss >> travelTime;

        g.add_edge(intersection1[0], intersection2[0], travelTime);
    }

    file.close();
}

int main() {
    Graph g;

    read_file(g, "road_network.csv");
    g.print_graph();

    return 0;
}
