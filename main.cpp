#include <functional>
#include <map>
#include <string>
#include <vector>

struct CollectionEntryData {
    std::string name;
    std::string surname;
};

struct CollectionEntry {
    const std::string UUID;
    CollectionEntryData d;

    CollectionEntry(const std::string& UUID, const CollectionEntryData& d) : UUID(UUID) {
        this->d = d;
    }
};

struct Collection {
private:
    std::vector<CollectionEntry> data;
    std::map<std::string, CollectionEntry*> search_index;
public:
    const CollectionEntry* insert(const std::string& UUID, const CollectionEntryData& data) {
        try {
            this->search_index.at(UUID);
        } catch (const std::out_of_range& e) {
            this->data.emplace_back(UUID, data);
            this->search_index[UUID] = &this->data.back();
            return &this->data.back();
        }
        return nullptr;
    }

    const CollectionEntry* get(const std::string& UUID) const {
        try {
            return this->search_index.at(UUID);
        } catch (const std::out_of_range& e) {
            return nullptr;
        }
    }

    bool modify(const std::string& UUID, std::function<void(CollectionEntryData&)> cb) {
        try {
            auto& entry = *this->search_index.at(UUID);
            cb(entry.d);
            return true;
        } catch (const std::out_of_range& e) {
            return false;
        }
    }

    bool erase(const std::string& UUID) {
        return false;
    }
};

int main() {
    Collection data;
    data.insert("vasya", {.name = "Vasya", .surname = "Pupkin"});
    data.insert("kolya", {.name = "Kolya", .surname = "Petrov"});
    data.modify("kolya", [](auto& v) {
        v.name = "Pippin";
    });
}
