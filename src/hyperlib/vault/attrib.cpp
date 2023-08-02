#include <hyperlib/vault/attrib.hpp>

namespace hyper
{
	auto attrib::node::get(attrib::collection* collection) const -> void*
	{
		return reinterpret_cast<void*(__thiscall*)(decltype(this), void*, attrib::collection*)>(0x00462BB0)(this, collection->layout, collection);
	}

	auto attrib::cls::collection(std::uint32_t collection_key) -> attrib::collection*
	{
		return collection::find(this->key, collection_key);
	}
}
