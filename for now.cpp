#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <chrono>
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
    Edge* next;
    int count;

    Edge() {
        count = 0;
        next = nullptr;
    }
    Edge(char s, char d, int w) {
        count = 0;
        src = s;
        dest = d;
        weight = w;
        next = nullptr;
    }
};

struct Vehicle {
    string id;
    char start;
    char end;
    Vehicle* next;
    Edge* path;

    Vehicle(string vid, char s, char e) {
        id = vid;
        start = s;
        end = e;
        next = nullptr;
        path = nullptr;
    }
    void add_path(Edge* path_add) {
        Edge* temp = path;
        if (!temp)
            path = path_add;
        else {
            while (temp->next)
                temp = temp->next;
            temp->next = path_add;
        }
    }
    bool remove_first_path(char id) {
        if (path) {
            if (path->src == id) {
                Edge* temp = path;
                path = temp->next;
                if (path)
                    ++path->count;
                delete temp;
                return true;
            }
        }
        return false;
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

    void add_vehicle(string i, char start, char end) {
        Vehicle* add_v = new Vehicle(i, start, end);
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
    void print_list() {
        Node* temp = adj_list;
        while (temp->next) {
            cout << temp->id << " -> ";
            temp = temp->next;
        }
        cout << temp->id << endl;
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
    void increase_count(char id) {
        Vertex* temp = head;
        while (temp) {
            if (temp->id == id) {
                ++temp->total_vehicles;
                return;
            }
            temp = temp->next;
        }
    }
    void check() {
        Vertex* temp = head;
        max = head->total_vehicles;
        while (temp) {
            if (temp->green_time <= 1 && temp->green_time > 10)
                temp->green_time -= 10;
            if (temp->green_time < 20 && temp->total_vehicles > 1)
                temp->green_time += 10;
            if (temp->total_vehicles > max) {
                head->next = temp->next;
                temp->next = head;
                head = temp;
                max = head->total_vehicles;
            }
            temp = temp->next;
        }
    }
    Vertex* dequeue() {
        if (head) {
            Vertex* to_deq = head;
            head = to_deq->next;
            check();
            cout << "queue now" << endl;
            print_queue();
            return to_deq;
        }
        return nullptr;
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
    Edge* edges[41];
    int count[41];
    bool exists[41];
    bool blocked[41];

    hash_table() {
        for (int i = 0; i < 41; i++) {
            count[i] = 0;
            exists[i] = false;
            blocked[41] = false;
        }
    }
    int get_hash(char src, char dest) {
        return (src * 31 + dest) % 41;
    }
    void insert_edge(Edge* to_add) {
        int hash_key = get_hash(to_add->src, to_add->dest);
        int start = hash_key;
        while (exists[hash_key]) {
            hash_key = (hash_key + 1) % 41;
            if (hash_key == start)
                return;
        }
        edges[hash_key] = to_add;
        exists[hash_key] = true;
    }
    Edge* find_edge(char src, char dest) {
        int key = get_hash(src, dest);
        return edges[key];
    }
    void add_veh_to_edge(Edge* edge_to_add) {
        int index = get_hash(edge_to_add->src, edge_to_add->dest);
        ++count[index];

    }
   /* void change_road_status(char s, char d) 
    {
        blocked[get_hash(s, d)] = true;
    }*/

 //  changing road status
    void change_road_status(char s, char d) {
        int key = get_hash(s, d);
        if (exists[key]) {
            blocked[key] = true;
            cout << "Road " << s << " -> " << d << " has been marked as blocked." << endl;
        }
        else {
            cout << "Road " << s << " -> " << d << " does not exist in the network!" << endl;
        }
    }

    // blocking edge dynamically
    void block_edge_dynamically( char src, char dest)
    {
        change_road_status(src, dest);
        cout << "Road from " << src << " to " << dest << " has been dynamically blocked!" << endl;
    }
 // printing all blocked roads
    void print_blocked_roads() 
    {
        cout << "Blocked Roads in the Network:" << endl;
        bool anyBlocked = false;

        for (int i = 0; i < 41; i++) {
            if (exists[i] && blocked[i]) {
                cout << edges[i]->src << " -> " << edges[i]->dest << endl;
                anyBlocked = true;
            }
        }

        if (!anyBlocked) {
            cout << "No roads are currently blocked." << endl;
        }
    }


    void print_table() {
        for (int i = 0; i < 41; i++) {
            cout << i << ": " << edges[i]->src << " " << edges[i]->dest << endl;
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
            if (!temp)
                vertices = new_vertex;
            else {
                while (temp->next)
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
            while (temp->next)
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
                while (temp->next)
                    temp = temp->next;
                temp->next = new_node;
            }
        }
        Edge* add_edge = new Edge(src, dest, weight);
        table.insert_edge(add_edge);
    }

    void initialize_queue() {
        Vertex* temp = vertices;
        while (temp) {
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

    void print_path(Vertex* dest_vertex, Vehicle* veh) {
        if (!dest_vertex->predecessor) {
            cout << dest_vertex->id << " ";
            return;
        }
        print_path(dest_vertex->predecessor, veh);
        cout << dest_vertex->id << " ";
        Edge* add_path = table.find_edge(dest_vertex->id, dest_vertex->predecessor->id);
        veh->add_path(add_path);
    }

    void dijkstra(char source, char dest, Vehicle* veh) 
    {
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
            temp = vertices;
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
                // adding condition for blocked roads
                if (table.blocked[table.get_hash(current->id, adj->id)]) {
                    cout << "Road blocked: " << current->id << " to " << adj->id << endl;
                    adj = adj->next;
                    continue;
                }

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
        Vertex* current = dest_vertex;
        while (current && current->predecessor) {
            Edge* edge = table.find_edge(current->predecessor->id, current->id);
            if (edge) {
                veh->add_path(new Edge(edge->src, edge->dest, edge->weight));
            }
            current = current->predecessor;
        }

        reverse_vehicle_path(veh);
        table.add_veh_to_edge(veh->path);
        print_vehicle_path(veh);
    }
    void reverse_vehicle_path(Vehicle* veh) {
        Edge* prev = nullptr;
        Edge* current = veh->path;
        Edge* next = nullptr;

        while (current) {
            next = current->next;
            current->next = prev;
            prev = current;
            current = next;
        }

        veh->path = prev;
    }
    void print_vehicle_path(Vehicle* veh) {
        Edge* current = veh->path;
        while (current) {
            cout << current->src << " -> " << current->dest << " (" << current->weight << ")";
            if (current->next) cout << " -> ";
            current = current->next;
        }
        cout << endl;
    }
    void find_path_for_all() {
        cout << "Paths found for all vehicles" << endl;
        Vehicle* temp = vehicles;
        while (temp) {
            cout << temp->id << " ";
            dijkstra(temp->start, temp->end, temp);
            temp = temp->next;
            cout << endl;
        }
    }
    void green_light() {
        Vehicle* current = vehicles;
        Vertex* removed = green_queue.dequeue();
        cout << "Intersection " << removed->id << " is opened/ green light" << endl;
        while (current) {
            if (current->remove_first_path(removed->id))
                if (current->path) {
                    green_queue.increase_count(current->path->dest);
                    cout << current->id << " has now current edge: " << current->path->src << " " << current->path->dest << endl;
                }
                else {
                    cout << current->id << " has completed it's path " << endl;

                }
            current = current->next;
        }
        delete removed;
    }
    void print_adj_list() {
        Vertex* temp = vertices;
        while (temp) {
            cout << temp->id << " -> ";
            temp->print_list();
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
            Vertex* to_delete = temp;
            temp = temp->next;
            delete to_delete;
        }
        // Vehicle* temp_v = vehicles;
        while (temp) {
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
    if (!file.is_open()) {
        cout << "Error opening file: " << filename << endl;
        return;
    }

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
// ___________________________reading blocked roads file_______________________//
void read_road_closures(Graph& g, const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Error opening file: " << filename << endl;
        return;
    }

    string line, intersection1, intersection2, status;

    getline(file, line); // Skip header line

    while (getline(file, line)) {
        stringstream ss(line);
        getline(ss, intersection1, ',');
        getline(ss, intersection2, ',');
        getline(ss, status, ',');

        if (status == "Blocked") {
            g.table.change_road_status(intersection1[0], intersection2[0]);
        }
    }

    file.close();
    cout << "Road closure data loaded successfully!" << endl;
}


int main()
{
    Graph g;
    /* read_file(g, "D:/Data Structure Labtasks/Data structure project/Data structure project/road_network.csv" );
     read_signals(g, "D:/Data Structure Labtasks/Data structure project/Data structure project/traffic_signals.csv");
     read_vehicles(g, "D:/Data Structure Labtasks/Data structure project/Data structure project/vehicles.csv");*/
     /*read_file(g, "C:/Users/hp/Desktop/uni sht/data struct/project/Project/road_network.csv");
     read_signals(g, "C:/Users/hp/Desktop/uni sht/data struct/project/Project/traffic_signals.csv");
     read_vehicles(g, "C:/Users/hp/Desktop/uni sht/data struct/project/Project/vehicles.csv");*/
    read_file(g, "road_network.csv");
    read_signals(g, "traffic_signals.csv");
    read_vehicles(g, "vehicles.csv");
    read_road_closures(g, "road_closures.csv");
    
    g.table.print_blocked_roads();
    g.table.block_edge_dynamically( 'B', 'F');
    g.find_path_for_all();
    g.table.print_blocked_roads();
    // g.print_graph();
    // g.print_vehicles();
    // g.dijkstra('A', 'F');

    // g.find_path_for_all();
    /*g.initialize_queue();*/
    // g.green_queue.print_queue();
    // g.table.print_table();
    // g.green_queue.check();
    // g.green_light();
    // g.print_adj_list();

    auto interval = chrono::seconds(g.green_queue.head->green_time);
    // cout << "Green time for current intersection: " << interval.count() << endl;
    auto start_time = chrono::steady_clock::now();

    while (true) {
        auto current_time = chrono::steady_clock::now();
        if (current_time - start_time >= interval) {
            start_time = current_time;
            g.green_light();
            interval = chrono::seconds(g.green_queue.head->green_time);
            cout << "Green time for current intersection: " << interval.count() << "s" << endl;
        }
    }

    return 0;
}
