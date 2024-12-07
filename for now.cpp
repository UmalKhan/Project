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
    int priority;

    Vehicle(string vid, char s, char e, int p) {
        id = vid;
        start = s;
        end = e;
        priority = p;
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
    Vertex* prev;
    int distance;
    bool visited;
    int green_time;
    Vertex* predecessor;
    Vehicle* vehicles_in;
    int total_vehicles;
    int heuristic;

    Vertex(char i) {
        id = i;
        adj_list = nullptr;
        next = nullptr;
        prev = nullptr;
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
        Vehicle* add_v = new Vehicle (i, start, end);
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
        tail = nullptr;
        max = 0;
    }
    void enqueue(Vertex* vertex_add) {
        Vertex* new_vertex = new Vertex(vertex_add);
        if (!head) {
            head = new_vertex;
            tail = new_vertex;
            head->next = tail;
            tail->prev = head;
            max = new_vertex->total_vehicles;
        }
        else {
            if (new_vertex->total_vehicles > max) {
                new_vertex->next = head;
                head->prev = new_vertex;
                head = new_vertex;
                max = new_vertex->total_vehicles;
            }
            else {
                tail->next = new_vertex;
                new_vertex->prev = tail;
                tail = new_vertex;
            }
        }
    }

    // change (i think only decrease) should depend on agr v less vehicles? for "less waiting time" for other intersections?
    void check() {
        cout << "check shuru" << endl;
        print_queue();
        // Vertex* temp = head;
        // max = head->total_vehicles;
        // while(temp) {
        //     if (temp->total_vehicles > max) {
        //         temp->next = head;
        //         head = temp;
        //         max = head->total_vehicles;
        //     }
        //     temp = temp->next;
        // }
        cout << "check" << endl;
    }

    // is ki call depends on our actual time? depends on hm simulate kesay kr rhe hain.
    // if it is actual time so we only have to consider front/head of queue ka time.
    // remove pehla path/edge of all- no. vehicles with that vertex as source. hmm.
    Vertex* dequeue() {   
        if (head) {
            Vertex* to_deq = head;
            head = to_deq->next;
            // check();
            return to_deq;
        }   
        return nullptr;                              
    }
    Vertex* find_intersection(char i) {
        Vertex* temp = head;
        while(temp && temp->id != i) 
            temp = temp->next;
        if (temp)
            return temp;
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
        Edge* found = edges[key];
        if (found->src == src && found->dest == dest)
            return found;
        return nullptr;
    }
    void add_veh_to_edge(Edge* edge_to_add) {
        int index = get_hash(edge_to_add->src, edge_to_add->dest);
        ++count[index];

    }
    void change_road_status(char s, char d) {
        blocked[get_hash(s, d)] = true;
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
    Vehicle* em_vehicles;
    int total_vertices;
    queue green_queue;
    hash_table table;

    Graph() {
        vehicles = nullptr;
        vertices = nullptr;
        em_vehicles = nullptr;
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

    void add_vehicle(string id, char start, char end, int prior) {
        Vehicle* new_vehicle = new Vehicle(id, start, end, prior);
        Vehicle* temp = vehicles;
        if (!temp)
            vehicles = new_vehicle;
        else {
            while (temp->next)
                temp = temp->next;
            temp->next = new_vehicle;
        }
        Vertex* src = find_vertex(start);
        src->add_vehicle(id, start, end, prior);
    }
    void add_emergency_vehicle(string id, char start, char end, int prior) {
        Vehicle* new_vehicle = new Vehicle(id, start, end, prior);
        Vehicle* temp = em_vehicles;
        if (!temp)
            em_vehicles = new_vehicle;
        else {
            while(temp->next)
                temp = temp->next;
            temp->next = new_vehicle;
        }
        Vertex* src = find_vertex(start);
        src->add_vehicle(id, start, end, prior);
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

    void dijkstra(char source, char dest, Vehicle* veh) {
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
    int heuristic(char current, char goal) {
        Edge* edge_d = table.find_edge(current, goal);
        int edge_distance;
        if (edge_d)
            edge_distance = edge_d->weight;
        else    
            return INT_MAX;
    
        Vertex* current_vertex = find_vertex(current);
        int congestion_penalty = current_vertex ? current_vertex->total_vehicles : 0;
        
        return edge_distance + current_vertex->total_vehicles;
    }
    void a_star(char source, char dest, Vehicle* veh) {
        Vertex* temp = vertices;
        veh->path = nullptr;
        while (temp) {
            temp->distance = 0;
            temp->heuristic = heuristic(source, temp->id);
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
        src_vertex->heuristic = 0;
        // src_vertex->heuristic = heuristic(source, dest);  // Initial heuristic

        while (true) {
            Vertex* current = nullptr;
            temp = vertices;
            while (temp) {
                int f_score = temp->distance + temp->heuristic;
                int current_f_score = current ? current->distance + current->heuristic : INT_MAX;
                if (!temp->visited && (current == nullptr || f_score < current_f_score)) {
                    current = temp;
                }
                temp = temp->next;
            }

            if (!current || current->distance == INT_MAX)
                break;

            if (current->id == dest)
                break;

            current->visited = true;

            Node* adj = current->adj_list;
            while (adj) {
                Vertex* neighbor = find_vertex(adj->id);
                if (neighbor && !neighbor->visited) {
                    int tentative_dist = current->distance + adj->weight;
                    if (tentative_dist > neighbor->distance) {
                        neighbor->distance = tentative_dist;
                        neighbor->heuristic = heuristic(neighbor->id, dest);
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
        // cout << "Path: ";
        
        Vertex* current = dest_vertex;
        while (current && current->predecessor) {
            Edge* edge = table.find_edge(current->predecessor->id, current->id);
            if (edge) {
                veh->add_path(new Edge(edge->src, edge->dest, edge->weight));
            }
            current = current->predecessor;
        }

        reverse_vehicle_path(veh);
        green_queue.emergency_rerouting(veh);
        table.add_veh_to_edge(veh->path);
        print_vehicle_path(veh);
    }
    void emergency_path(Vehicle* e_veh) {
        a_star(e_veh->start, e_veh->end, e_veh);
    }


    void find_path_for_all() {
        cout << "Paths found for all vehicles" << endl;
        Vehicle* temp = vehicles;
        cout << temp->id << " ";
        while (temp) {
            cout << temp->id << " ";
            dijkstra(temp->start, temp->end, temp);
            // a_star(temp->start, temp->end, temp);
            temp = temp->next;
            cout << endl;
        }
    }
    void green_light() {
        Vehicle* current = vehicles;
        Vertex* removed = green_queue.dequeue();
        while(current) {
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
        // Vehicle* temp_v = vehicles;
        while (temp) {
            Vertex* to_delete = temp;
            temp = temp->next;
            delete to_delete;
        }
    }
};

void read_emergency_vehicles(Graph& graph, const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Error opening file: " << filename << endl;
        return;
    }

    string line, vehicleID, priorityLevel;
    char startIntersection, endIntersection;

    getline(file, line);

    while (getline(file, line)) {

        if (line.empty()) {
            continue;
        }

        stringstream ss(line);

        if (line.empty() || line.find_first_not_of(",") == string::npos)
            continue;

        getline(ss, vehicleID, ',');
        ss >> startIntersection; ss.ignore(1, ',');
        ss >> endIntersection; ss.ignore(1, ',');
        getline(ss, priorityLevel, ',');

        int priority;
        if (priorityLevel == "Medium")
            priority = 1;
        else
            priority = 2;

        graph.add_emergency_vehicle(vehicleID, startIntersection, endIntersection, priority);
    }

    file.close();
}

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

        graph.add_vehicle(vehicleID, startIntersection, endIntersection, 0);
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

    read_file(g, "C:/Users/hp/Desktop/uni sht/data struct/project/Project/road_network.csv");
    read_signals(g, "C:/Users/hp/Desktop/uni sht/data struct/project/Project/traffic_signals.csv");
    read_vehicles(g, "C:/Users/hp/Desktop/uni sht/data struct/project/Project/vehicles.csv");
    // g.print_graph();
    // g.print_vehicles();
    // g.print_vehicles_at_vertex();

    g.find_path_for_all();
    g.initialize_queue();
    // g.green_queue.print_queue();
    // g.table.print_table();
    g.green_light();
    return 0;
}
