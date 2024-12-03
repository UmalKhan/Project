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

struct Edge {
    char src;
    char dest;
    int weight;

    Edge() {
        src = ' ';
        dest = ' ';
        weight = 0;
    }

    Edge(char s, char d, int w) {
        src = s;
        dest = d;
        weight = w;
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

struct Vertex {
    char id;
    Node* adj_list;
    Vertex* next;
    int distance;
    bool visited;
    int green_time;
    Vertex* predecessor;
    Vehicle* vehicles_in;
    int total_vehicles;

    Vertex(char i) {
        id = i;
        adj_list = nullptr;
        next = nullptr;
        distance = INT_MAX;
        visited = false;
        predecessor = nullptr;
        vehicles_in = nullptr;
        total_vehicles = 0;
    }

    Vertex(Vertex* to_copy) {
        id = to_copy->id;
        adj_list = to_copy->adj_list;
        distance = to_copy->distance;
        visited = to_copy->visited;
        predecessor = to_copy->predecessor;
        vehicles_in = to_copy->vehicles_in;
        total_vehicles = to_copy->total_vehicles;
        green_time = to_copy->green_time;
        next = nullptr;
    }

    void add_vehicle(string id, char start, char end) {
        Vehicle* add_v = new Vehicle (id, start, end);
        Vehicle* temp = vehicles_in;
        if (!temp) 
            vehicles_in = add_v;
        else {
            while (temp->next)
                temp = temp->next;
            temp->next = add_v;
        }
        ++total_vehicles;
    }
};

struct queue {
    Vertex* head;
    Vertex* tail;
    int max;

    queue() {
        head = nullptr;
        max = 0;
    }
    void enqueue(Vertex* vertex_add) {
        Vertex* new_vertex = new Vertex(vertex_add);
        if (!head) {
            head = new_vertex;    
            tail = new_vertex;
            max = new_vertex->total_vehicles;
        }
        else {
            if (new_vertex->total_vehicles > max) {
                new_vertex->next = head;
                head = new_vertex;
                max = new_vertex->total_vehicles;
            }
            else {
                tail->next = new_vertex;
                tail = new_vertex;
            }
        }
    }
    void check() {                                      // change (i think only decrease) should depend on agr v less vehicles? for "less waiting time" for other intersections?
        Vertex* temp = head;
        max = head->total_vehicles;
        while(temp) {
            if (temp->total_vehicles > max) {
                temp->next = head;
                head = temp;
                max = temp->total_vehicles;
            }
        }
    }
    void dequeue() {                                    // is ki call depends on our actual time? depends on hm simulate kesay kr rhe hain.
        Vertex* to_deq = head;                          // if it is actual time so we only have to consider front/head of queue ka time.
        head = to_deq->next;
        delete to_deq;
        check();
    }
    void print_queue() {
        Vertex* temp = head;
        while (temp) {
            cout << temp->id << ": " << temp->green_time << "s" << " " << temp->total_vehicles << endl;
            temp = temp->next;
        }
    }
};

struct hash_table {
    Edge edges[50];
    bool exists[50];
    
    hash_table() {
        for (int i = 0; i < 50; i++) {
            exists[i] = false;
        }
    }
    int get_hash(Edge hash_val) {
        return (hash_val.src + hash_val.dest) % 50;
    }
    void insert_edge(Edge to_add) {
        int hash_key = get_hash(to_add);
        int start = hash_key;
        while(exists[hash_key]) {
            hash_key = (hash_key + 1) % 50;
            if (hash_key == start)
                return;
        }
        edges[hash_key] = to_add;
        exists[hash_key] = true;
    }
    void print_table() {
        for (int i = 0; i < 50; i++) {
            cout << i << ": " << edges[i].src << " " << edges[i].dest << endl;
        }
    }
};

struct Graph {
    Vertex* vertices;
    Vehicle* vehicles;
    int total_vertices;
    queue green_queue;
    hash_table table;

    Graph() {
        vehicles = nullptr;
        vertices = nullptr;
        total_vertices = 0;
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
            ++total_vertices;
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
        Vertex* src = find_vertex(start);
        src->add_vehicle(id, start, end);
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
        Edge add_edge(src, dest, weight);
        table.insert_edge(add_edge);
    }

    void initialize_queue() {
        Vertex* temp = vertices;
        while(temp) {
            green_queue.enqueue(temp);
            temp = temp->next;
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
            cout << temp->id;;
            Node* adj = temp->adj_list;
            while (adj) {
                cout << " -> " << adj->id << "(" << adj->weight << ")";
                adj = adj->next;
            }
            cout << endl;
            cout << "Vehicles at this vertex: ";
            Vehicle* vertex_v = temp->vehicles_in;
            while (vertex_v) {
                cout << vertex_v->id << " ";
                vertex_v = vertex_v->next;
            }
            cout << endl << "Green time: " << temp->green_time << endl;
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

    // g.find_path_for_all();
    // g.initialize_queue();
    // g.green_queue.print_queue();
    g.table.print_table();
    return 0;
}
