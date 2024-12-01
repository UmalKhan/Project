#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
using namespace std;

struct Node {
    char id;
    int weight;
    Node* next;

    Node(char i, int w) {
        id = i;
        weight = w;
        next = nullptr;
    } 
};

struct Vertex {
    char id;
    Node* adj_list;
    Vertex* next;
    int distance;
    bool visited;
    int green_time;
    Vertex* predecessor;

    Vertex(char i) {
        id = i;
        adj_list = nullptr;
        next = nullptr;
        distance = INT_MAX;
        visited = false;
        predecessor = nullptr;
    }
};

struct Vehicle {
    string id;
    char start;
    char end;
    Vehicle* next;

    Vehicle(string vid, char s, char e) {
        id = vid;
        start = s;
        end = e;
        next = nullptr;
    }
};


struct Graph {
    Vertex* vertices;
    Vehicle* vehicles;

    Graph() {
        vehicles = nullptr;
        vertices = nullptr;
    }

    void add_vertex(char id) {
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

    void add_vehicle(string id, char start, char end) {
        Vehicle* new_vehicle = new Vehicle(id, start, end);
        Vehicle* temp = vehicles;
        if (!temp)
            vehicles = new_vehicle;
        else {
            while(temp->next)
                temp = temp->next;
            temp->next = new_vehicle;
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
        add_vertex(src);
        add_vertex(dest);

        Vertex* src_vertex = find_vertex(src);
        if (src_vertex) {
            Node* new_node = new Node(dest, weight);
            Node* temp = src_vertex->adj_list;
            if (!temp)
                src_vertex->adj_list = new_node;
            else {
                while(temp->next)
                    temp = temp->next;
                temp->next = new_node;
            }
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

    void print_vehicles() {
        Vehicle* temp = vehicles;
        while (temp) {
            cout << temp->id << " from " << temp->start << " to " << temp->end << endl;
            temp = temp->next;
        }
    }

    void print_graph() {
        Vertex* temp = vertices;
        while (temp) {
            cout << temp->id << " (" << temp->green_time << "s) "<< ":";
            Node* adj = temp->adj_list;
            while (adj) {
                cout << " -> " << adj->id << "(" << adj->weight << ")";
                adj = adj->next;
            }
            cout << endl;
            temp = temp->next;
        }
    }

    void set_time(char id, int greenTime) {
        Vertex* vertex = find_vertex(id);
        if (vertex)
            vertex->green_time = greenTime;
        else
            cout << "Vertex " << id << " not found!\n";
    }

    void print_path(Vertex* dest_vertex) {
        if (!dest_vertex) 
            return;
        print_path(dest_vertex->predecessor);
        cout << dest_vertex->id << " ";
    }


    void dijkstra(char source, char dest) {
        Vertex* temp = vertices;
        while (temp) {
            temp->distance = INT_MAX;
            temp->visited = false;
            temp->predecessor = nullptr;
            temp = temp->next;
        }
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
                if (!temp->visited && (current == nullptr || temp->distance < current->distance))
                    current = temp;
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
                        neighbor->predecessor = current;
                    }
                }
                adj = adj->next;
            }
        }

        Vertex* dest_vertex = find_vertex(dest);
        if (!dest_vertex || dest_vertex->distance == INT_MAX) {
            cout << "No path found from " << source << " to " << dest << endl;
            return;
        }

        cout << "Shortest distance from " << source << " to " << dest << ": " << dest_vertex->distance << endl;
        cout << "Path: ";
        print_path(dest_vertex);
        cout << endl;
    }

    void find_path_for_all() {
        Vehicle* temp = vehicles;
        while (temp) {
            cout << temp->id << " ";
            dijkstra(temp->start, temp->end);
            temp = temp->next;
            cout << endl;
        }
    }

    // void dijkstra(char source) {
    //     Vertex* src_vertex = find_vertex(source);
    //     if (!src_vertex) {
    //         cout << "Source vertex not found!" << endl;
    //         return;
    //     }

    //     src_vertex->distance = 0;

    //     while (true) {
    //         Vertex* current = nullptr;
    //         Vertex* temp = vertices;
    //         while (temp) {
    //             if (!temp->visited && (current == nullptr || temp->distance < current->distance)) {
    //                 current = temp;
    //             }
    //             temp = temp->next;
    //         }

    //         if (!current || current->distance == INT_MAX) 
    //             break;

    //         current->visited = true;

    //         Node* adj = current->adj_list;
    //         while (adj) {
    //             Vertex* neighbor = find_vertex(adj->id);
    //             if (neighbor && !neighbor->visited) {
    //                 int new_dist = current->distance + adj->weight;
    //                 if (new_dist < neighbor->distance) {
    //                     neighbor->distance = new_dist;
    //                 }
    //             }
    //             adj = adj->next;
    //         }
    //     }

    //     cout << "Shortest distances from vertex " << source << ":\n";
    //     Vertex* temp = vertices;
    //     while (temp) {
    //         cout << "To " << temp->id << ": ";
    //         if (temp->distance == INT_MAX) {
    //             cout << "INF\n";
    //         } else {
    //             cout << temp->distance << "\n";
    //         }
    //         temp = temp->next;
    //     }
    // }


    ~Graph() {
        Vertex* temp = vertices;
        while (temp) {
            Node* adj = temp->adj_list;
            while (adj) {
                Node* toDelete = adj;
                adj = adj->next;
                delete toDelete;
            }
            Vertex* to_delete = temp;
            temp = temp->next;
            delete to_delete;
        }
        Vehicle* temp_v = vehicles;
        while(temp) {
            Vertex* to_delete = temp;
            temp = temp->next;
            delete to_delete;
        }
    }
};

void read_vehicles(Graph& graph, const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Error opening file: " << filename << endl;
        return;
    }

    string line, vehicleID;
    char startIntersection, endIntersection;

    getline(file, line);

    while (getline(file, line)) {
        stringstream ss(line);

        getline(ss, vehicleID, ',');
        ss >> startIntersection; ss.ignore(1, ',');
        ss >> endIntersection;

        graph.add_vehicle(vehicleID, startIntersection, endIntersection);
    }

    file.close();
}


void read_signals(Graph& graph, const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Error opening file: " << filename << endl;
        return;
    }

    string line, intersection;
    int greenTime;

    getline(file, line);

    while (getline(file, line)) {
        stringstream ss(line);
        getline(ss, intersection, ',');
        ss >> greenTime;

        if (!intersection.empty()) {
            graph.set_time(intersection[0], greenTime);
        }
    }

    file.close();
}


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
    read_signals(g, "traffic_signals.csv");
    read_vehicles(g, "vehicles.csv");
    // g.print_graph();
    // g.print_vehicles();
    // g.dijkstra('A', 'F');

    g.find_path_for_all();

    return 0;
}
