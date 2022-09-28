#include <any>
#include <utility>
#include <vector>

namespace bm::data
{
class registry
{
private:
    std::vector<std::any> storage;

public:
    template <typename T> std::size_t register_owner(T&& owner)
    {
        storage.emplace_back(std::forward<T>(owner));
        return size(storage) - 1;
    }

    void unregister_owner(std::size_t key)
    {
        storage.at(key).reset();
    }

    void unregister_back()
    {
        if (!storage.empty())
            storage.back().reset();
    }

    template <typename T> T* at(std::size_t key)
    {
        return std::any_cast<T>(&storage.at(key));
    }

    template <typename T> T* back()
    {
        if (storage.empty())
            return nullptr;
        return std::any_cast<T>(&storage.back());
    }
};
} // namespace bm::data