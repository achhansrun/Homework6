#pragma once
#include "llnode2.hpp"
#include <memory>
#include <stdexcept>
#include <cstddef>

template<typename ValType>
void reverseList(std::unique_ptr<LLNode2<ValType>>& head) {
	std::unique_ptr<LLNode2<ValType>> prev = nullptr;
	std::unique_ptr<LLNode2<ValType>> current = std::move(head);

	while (current) {
		std::unique_ptr<LLNode2<ValType>> next = std::move(current->_next);
		current->_next = std::move(prev);
		prev = std::move(current);
		current = std::move(next);
	}

	head = std::move(prev);
}


template<typename KeyType, typename DataType>
class SlowMap {
public:
	template<typename KVType>
	struct LLNode2 {
		KVType data;
		std::unique_ptr<LLNode2<KVType>> next;

		LLNode2(const KVType& val) : data(val), next(nullptr) {}
	};

	using KVType = std::pair<KeyType, DataType>;
	using Node = LLNode2<KVType>;
	using size_type = std::size_t;
	std::unique_ptr<Node> head;

public:
	SlowMap() = default;

	~SlowMap() = default;

	 size_type size() const {
		size_type count = 0;
		Node* current = head.get();
		while (current) {
			++count;
			current = current->next.get();
		}
		return count;
	}

	bool empty() const {
		return size() == 0;
	}

	bool present(const KeyType& key) const {
		Node* current = head.get();
		while (current) {
			if (current->data.first == key) {
				return true;
			}
			current = current->next.get();
		}
		return false;
	}

	const DataType& get(const KeyType& key) const {
		Node* current = head.get();
		while (current) {
			if (current->data.first == key) {
				return current->data.second;
			}
			current = current->next.get();
		}
		throw std::out_of_range("Key not found in SlowMap");
	}

	DataType& get(const KeyType& key) {
		return const_cast<DataType&>(const_cast<const SlowMap*>(this)->get(key));
	}

	void set(const KeyType& key, const DataType& value) {
		Node* current = head.get();
		while (current) {
			if (current->data.first == key) {
				current->data.second = value;
				return;
			}
			current = current->next.get();
		}

		auto newNode = std::make_unique<Node>(std::make_pair(key, value));
		newNode->next = std::move(head);
		head = std::move(newNode);
	}

	void erase(const KeyType& key) {
		Node* current = head.get();
		Node* prev = nullptr;

		while (current) {
			if (current->data.first == key) {
				if (prev) {
					prev->next = std::move(current->next);
				}
				else {
					head = std::move(current->next);
				}
				return;
			}

			prev = current;
			current = current->next.get();
		}
	}

	template<typename Function>
	void traverse(Function&& func) const {
		Node* current = head.get();
		while (current) {
			func(current->data.first, current->data.second);
			current = current->next.get();
		}
	}
};;