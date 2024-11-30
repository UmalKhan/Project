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
    int distance;
    bool visited;

    Vertex(char i) {
        id = i;
        adj_list = nullptr;
        next = nullptr;
        distance = INT_MAX;
        visited = false;
    }
};

struct Graph {
    Vertex* vertices;

    Graph() {
        vertices = nullptr;
    }

    void addVertex(char id) {
        if (!vertex_exists(id)) {
            Vertex* new_vertex = new Vertex(id);
            Vertex* temp = vertices;
            if(!temp)
                vertices = new_vertex;
            else {
                while(temp->next)
                    temp = temp->next;
                temp->next = new_vertex;
            }
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

        Vertex* src_vertex = find_vertex(src);
        if (src_vertex) {
            Node* new_node = new Node(dest, weight, src_vertex->adj_list);
            src_vertex->adj_list = new_node;
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

    void dijkstra(char source) {
        Vertex* src_vertex = find_vertex(source);
        if (!src_vertex) {
            cout << "Source vertex not found!" << endl;
            return;
        }

        src_vertex->distance = 0;

        while (true) {
            Vertex* current = nullptr;
            Vertex* temp = vertices;
            while (temp) {
                if (!temp->visited && (current == nullptr || temp->distance < current->distance)) {
                    current = temp;
                }
                temp = temp->next;
            }

            if (!current || current->distance == INT_MAX) 
                break;

            current->visited = true;

            Node* adj = current->adj_list;
            while (adj) {
                Vertex* neighbor = find_vertex(adj->id);
                if (neighbor && !neighbor->visited) {
                    int new_dist = current->distance + adj->weight;
                    if (new_dist < neighbor->distance) {
                        neighbor->distance = new_dist;
                    }
                }
                adj = adj->next;
            }
        }

        cout << "Shortest distances from vertex " << source << ":\n";
        Vertex* temp = vertices;
        while (temp) {
            cout << "To " << temp->id << ": ";
            if (temp->distance == INT_MAX) {
                cout << "INF\n";
            } else {
                cout << temp->distance << "\n";
            }
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

    g.dijkstra('X');

    return 0;
}
