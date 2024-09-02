#include <iostream>
#include <map>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <utility>
using namespace std;

int num_websites;

struct Website
{
    int website_id;
    string owner_id;
    int bandwidth;
    int processing_power;
    vector<pair<int, int>> request_queue;
};

struct HttpRequest
{
    int request_id;
    int website_id;
    int processing_time;
};

class LoadBalancer
{
public:
    LoadBalancer() : global_time(0) {}

    void addWebsite(int website_id, const string &owner_id, int bandwidth, int processing_power)
    {
        Website *website = new Website();
        website->website_id = website_id;
        website->owner_id = owner_id;
        website->bandwidth = bandwidth;
        website->processing_power = processing_power;
        websites[website_id] = website;
    }

    void enqueueRequest(HttpRequest *http_request, int time)
    {
        Website *website = websites[http_request->website_id];
        requests[http_request->request_id] = http_request;
        website->request_queue.emplace_back(http_request->request_id, time);
    }

    void dequeueRequests(int power_divisor)
    {
        vector<pair<double, int>> weights;
        for (const auto &website : websites)
        {
            double w = website.second->bandwidth + website.second->processing_power;
            weights.emplace_back(w, website.first);
        }

        unordered_map<int, double> band;
        for (const auto &website : websites)
        {
            double w = website.second->bandwidth + website.second->processing_power;
            band[website.first] = w / power_divisor;
        }

        unordered_map<int, double> processing_times_temp;
        unordered_map<int, double> real_processing_times;
        for (const auto &website : websites)
        {
            vector<pair<int, int>> reqs = website.second->request_queue;
            double t = 0;
            for (const auto &req : reqs)
            {
                t = max(t, static_cast<double>(req.second)) + static_cast<double>(requests[req.first]->processing_time) / band[website.first];
                processing_times_temp[req.first] = t;
                real_processing_times[req.first] = t;
            }
        }

        vector<pair<double, int>> processing_times;
        for (const auto &entry : processing_times_temp)
        {
            processing_times.emplace_back(entry.second, entry.first);
        }
        sort(processing_times.begin(), processing_times.end());

        for (const auto &entry : processing_times)
        {
            int id = entry.second;
            cout << "HTTP Request with ID #" << id << " is processed at time " << real_processing_times[id] << '\n';
        }
    }

private:
    int global_time;
    map<int, Website *> websites;
    map<int, HttpRequest *> requests;
};

int main()
{
    LoadBalancer lb;
    int num_requests = 0;

    cout << "Enter the number of websites: ";
    cin >> num_websites;
    cout << "Enter the total number of requests: ";
    cin >> num_requests;

    for (int i = 0; i < num_websites; ++i)
    {
        cout << "Entry for website " << i + 1 << ":\n";
        int website_id;
        string owner_id;
        int bandwidth, processing_power;

        cout << "Enter website ID: ";
        cin >> website_id;
        cout << "Enter owner ID: ";
        cin >> owner_id;
        cout << "Enter bandwidth: ";
        cin >> bandwidth;
        cout << "Enter processing power: ";
        cin >> processing_power;
        cout << '\n';

        lb.addWebsite(website_id, owner_id, bandwidth, processing_power);
    }

    cout << '\n';

    for (int i = 0; i < num_requests; ++i)
    {
        cout << "Entry for request " << i + 1 << ":\n";
        HttpRequest *http_req = new HttpRequest();
        int req_id, web_id, arrival_time, process_time;

        cout << "Enter request ID: ";
        cin >> req_id;
        cout << "Enter website ID: ";
        cin >> web_id;
        cout << "Enter arrival time: ";
        cin >> arrival_time;
        cout << "Enter processing time: ";
        cin >> process_time;
        cout << '\n';

        http_req->website_id = web_id;
        http_req->request_id = req_id;
        http_req->processing_time = process_time;

        lb.enqueueRequest(http_req, arrival_time);
    }

    int power_divisor;
    cout << "Enter power divisor: ";
    cin >> power_divisor;
    cout << "Processing the requests...\n";

    lb.dequeueRequests(power_divisor);

    return 0;
}
