#pragma once

#include <cassert>
#include <cstddef>
#include <string>
#include <utility>

template <typename Type>
class SingleLinkedList
{
    struct Node
    {
        Node() = default;
        Node(const Type &val, Node *next)
            : value(val), next_node(next)
        {
        }
        Type value;
        Node *next_node = nullptr;
    };
    
    template <typename ValueType>
    class BasicIterator
    {
        friend class SingleLinkedList;
        
        explicit BasicIterator(Node *node)
        {
            node_ = node;
        }

    public:
        using iterator_category = std::forward_iterator_tag;
        
        using value_type = Type;
        
        using difference_type = std::ptrdiff_t;
        
        using pointer = ValueType *;
        
        using reference = ValueType &;

        BasicIterator() = default;

        BasicIterator(const BasicIterator<Type> &other) noexcept
        {
            node_ = other.node_;
        }

        BasicIterator &operator=(const BasicIterator &rhs) = default;

        [[nodiscard]] bool operator==(const BasicIterator<const Type> &rhs) const noexcept
        {
            return this->node_ == rhs.node_;
        }

        [[nodiscard]] bool operator!=(const BasicIterator<const Type> &rhs) const noexcept
        {
            return this->node_ != rhs.node_;
        }

        [[nodiscard]] bool operator==(const BasicIterator<Type> &rhs) const noexcept
        {
            return this->node_ == rhs.node_;
        }

        [[nodiscard]] bool operator!=(const BasicIterator<Type> &rhs) const noexcept
        {
            return this->node_ != rhs.node_;
        }

        BasicIterator &operator++() noexcept
        {
            *this = Iterator{node_->next_node};
            return *this;
        }

        BasicIterator operator++(int) noexcept
        {
            auto value(*this);
            ++(*this);
            return value;
        }

        [[nodiscard]] reference operator*() const noexcept
        {
            return node_->value;
        }

        [[nodiscard]] pointer operator->() const noexcept
        {
            return &(node_->value);
        }

    private:
        Node *node_ = nullptr;
    };

public:
    SingleLinkedList()
    {
        size_ = 0;
    }

    ~SingleLinkedList()
    {
        Clear();
    }

    SingleLinkedList(std::initializer_list<Type> values)
    {
        head_.next_node = nullptr;
        size_ = values.size();
        Node **temp = &head_.next_node;
        for (auto value : values)
        {
            *temp = new Node(value, nullptr);
            temp = &((*temp)->next_node);
        }
    }

    SingleLinkedList(const SingleLinkedList &other)
    {
        head_.next_node = nullptr;
        size_ = other.size_;
        Node **temp = &head_.next_node;
        for (auto value : other)
        {
            *temp = new Node(value, nullptr);
            temp = &((*temp)->next_node);
        }
    }

    SingleLinkedList &operator=(const SingleLinkedList &rhs)
    {
        if (this != &rhs)
        {
            SingleLinkedList temp(rhs);
            this->swap(temp);
        }
        return *this;
    }

    void swap(SingleLinkedList &other) noexcept
    {
        std::swap(other.size_, size_);
        std::swap(other.head_.next_node, head_.next_node);
    }

    void PushFront(const Type &value)
    {
        head_.next_node = new Node(value, head_.next_node);
        ++size_;
    }

    void Clear()
    {
        while (head_.next_node != nullptr)
        {
            auto temp = head_.next_node->next_node;
            delete head_.next_node;
            head_.next_node = temp;
        }
        size_ = 0;
    }

    [[nodiscard]] size_t GetSize() const noexcept
    {
        return size_;
    }

    [[nodiscard]] bool IsEmpty() const noexcept
    {
        if (size_ == 0)
        {
            return true;
        }
        return false;
    }

    using value_type = Type;

    using reference = value_type &;

    using const_reference = const value_type &;

    using Iterator = BasicIterator<Type>;
    
    using ConstIterator = BasicIterator<const Type>;

    [[nodiscard]] Iterator begin() noexcept
    {
        return Iterator{head_.next_node};
    }

    [[nodiscard]] Iterator end() noexcept
    {
        auto current_element = head_.next_node;
        while (current_element != nullptr)
        {
            current_element = current_element->next_node;
        }

        return Iterator{current_element};
    }

    [[nodiscard]] ConstIterator begin() const noexcept
    {
        return ConstIterator{head_.next_node};
    }

    [[nodiscard]] ConstIterator end() const noexcept
    {
        auto current_element = head_.next_node;
        while (current_element != nullptr)
        {
            current_element = current_element->next_node;
        }
        return ConstIterator{current_element};
    }

    [[nodiscard]] ConstIterator cbegin() const noexcept
    {
        return ConstIterator{head_.next_node};
    }

    [[nodiscard]] ConstIterator cend() const noexcept
    {
        auto current_element = head_.next_node;
        while (current_element != nullptr)
        {
            current_element = current_element->next_node;
        }
        return ConstIterator{current_element};
    }

    [[nodiscard]] Iterator before_begin() noexcept
    {
        return Iterator{&head_};
    }

    [[nodiscard]] ConstIterator cbefore_begin() const noexcept
    {
        return ConstIterator{const_cast<Node *>(&head_)};
    }

    [[nodiscard]] ConstIterator before_begin() const noexcept
    {
        return ConstIterator{const_cast<Node *>(&head_)};
    }

    Iterator InsertAfter(ConstIterator pos, const Type &value)
    {
        auto temp = pos.node_->next_node;
        pos.node_->next_node = new Node(value, temp);
        ++size_;
        return Iterator{pos.node_->next_node};
    }

    void PopFront() noexcept
    {
        Node *temp = head_.next_node;
        head_.next_node = head_.next_node->next_node;
        delete temp;
        --size_;
    }

    Iterator EraseAfter(ConstIterator pos) noexcept
    {
        Node *temp = pos.node_->next_node;
        pos.node_->next_node = pos.node_->next_node->next_node;
        delete temp;
        --size_;
        return Iterator{pos.node_->next_node};
    }

private:
    Node head_;
    size_t size_;
};

template <typename Type>
void swap(SingleLinkedList<Type> &lhs, SingleLinkedList<Type> &rhs) noexcept
{
    lhs.swap(rhs);
}

template <typename Type>
bool operator==(const SingleLinkedList<Type> &lhs, const SingleLinkedList<Type> &rhs)
{
    return std::equal(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

template <typename Type>
bool operator!=(const SingleLinkedList<Type> &lhs, const SingleLinkedList<Type> &rhs)
{
    return !(lhs == rhs);
}

template <typename Type>
bool operator<(const SingleLinkedList<Type> &lhs, const SingleLinkedList<Type> &rhs)
{
    return lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

template <typename Type>
bool operator>(const SingleLinkedList<Type> &lhs, const SingleLinkedList<Type> &rhs)
{
    return rhs < lhs;
}

template <typename Type>
bool operator<=(const SingleLinkedList<Type> &lhs, const SingleLinkedList<Type> &rhs)
{
    return !(lhs > rhs);
}

template <typename Type>
bool operator>=(const SingleLinkedList<Type> &lhs, const SingleLinkedList<Type> &rhs)
{
    return !(lhs < rhs);
}
