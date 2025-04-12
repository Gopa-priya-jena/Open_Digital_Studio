#pragma once
#include <DEBUG.hpp>
#include <cstddef>
#include <cstdint>
#include <cstdlib>

namespace DATA_STRUCTURE
{
// this is a  linked list of arrays this designend for perfomance as comapred to
// normal dynamic allocation
template <class Data_Type>
struct Dynamic_Array_metadata
{
    Dynamic_Array_metadata* Next{nullptr};
    Dynamic_Array_metadata* Previous{nullptr};
    std::uint32_t           Base_Index{0};
    std::uint32_t           Capacity{0};
    Data_Type               Data[];
};

template <class Data_Type>
class Dynamic_Array
{  // pointer to current  metadata contained by the  linked list
    Dynamic_Array_metadata<Data_Type>* metadata{nullptr};
    // needed for efficiency
    // max  possible index
    unsigned long long int Max_Index{0};
    // last index that has been entered
    signed long long Last_index{-1};
    // afuncton pointer to change growth if provided by the user
    std::uint64_t (*Grow_policy)(unsigned long long,
                                 unsigned long long) = nullptr;
    std::uint64_t grow_policy();

    void Grow(std::uint_fast64_t Given_Size = 0);

    void node_initialize(Dynamic_Array_metadata<Data_Type>*       present,
                         Dynamic_Array_metadata<Data_Type>* const next,
                         Dynamic_Array_metadata<Data_Type>* const previous,
                         const std::uint32_t                      base_Index,
                         const std::uint32_t                      capacity);

    void forward_goto_position(const std::uint_fast64_t position);

    void backward_goto_position(const std::uint_fast64_t position);

   public:
    Dynamic_Array();

    Dynamic_Array(const Data_Type* data, std::size_t Size);
    void              reserve(const unsigned int Size);
    const Data_Type&& operator=(const Data_Type&& data);

    Data_Type& get_data_at(const std::uint64_t position);

    Data_Type& operator[](const std::uint64_t position);

    // Modifiers

    // sets the contents

    void set(const Data_Type&& data);

    // bool insert();

    // inserts elements
    // bool insert_range();

    // inserts a range of elements
    // bool emplace();

    // constructs element in-place
    // bool erase();

    // erases elements
    void push_back(const Data_Type& data);

    const Data_Type pop_back();
    // removes the last element

    void Register_grow_policy(std::uint64_t (*policy)(unsigned long long,
                                                      unsigned long long));

    const std::uint_fast64_t Max_element();

    // TODO::completed  insertion of the data
    void insert(const std::uint_fast64_t position,
                const Data_Type&&        data,
                const int                Size);
};

}  // namespace DATA_STRUCTURE
// implementations
namespace DATA_STRUCTURE
{

template <class Data_Type>
std::uint64_t Dynamic_Array<Data_Type>::grow_policy()
{
    if (Grow_policy == nullptr)
        return (2 * Max_Index);
    else
        return Grow_policy(Max_Index, Last_index);
}

template <class Data_Type>
void Dynamic_Array<Data_Type>::Grow(std::uint_fast64_t Given_Size)
{
    FUNCTION_PROFILE

    LOG("ok");

    while (metadata->Next != nullptr)
    {
        LOG("next metadata");

        metadata = metadata->Next;
    }

    LOG("ok2");

    auto Size = (Given_Size > 0) ? Given_Size : grow_policy();

    metadata->Next = static_cast<Dynamic_Array_metadata<Data_Type>*>(
        malloc(sizeof(Dynamic_Array_metadata<Data_Type>) +
               (Size * sizeof(Data_Type))));

    node_initialize(metadata->Next,
                    nullptr,
                    metadata,
                    metadata->Capacity + metadata->Base_Index,
                    Size);
    Max_Index = metadata->Next->Base_Index + metadata->Next->Capacity;
}
template <class Data_Type>
void Dynamic_Array<Data_Type>::node_initialize(
    Dynamic_Array_metadata<Data_Type>*       present,
    Dynamic_Array_metadata<Data_Type>* const next,
    Dynamic_Array_metadata<Data_Type>* const previous,
    const std::uint32_t                      base_Index,
    const std::uint32_t                      capacity)
{
    LOG("new node created")
    present->Next = next;
    present->Previous = previous;
    present->Base_Index = base_Index;
    present->Capacity = capacity;
}
template <class Data_Type>
void Dynamic_Array<Data_Type>::forward_goto_position(
    const std::uint_fast64_t position)
{
    FUNCTION_PROFILE

    while ((metadata->Next != nullptr) &&
           (position >= metadata->Base_Index + metadata->Capacity))
    {
        metadata = metadata->Next;
    }
}

template <class Data_Type>
void Dynamic_Array<Data_Type>::backward_goto_position(
    const std::uint_fast64_t position)
{
    FUNCTION_PROFILE

    while ((metadata->Previous != nullptr) && (position < metadata->Base_Index))
    {
        metadata = metadata->Previous;
    }
}

template <class Data_Type>
Dynamic_Array<Data_Type>::Dynamic_Array() : metadata(nullptr)
{
}

template <class Data_Type>
Dynamic_Array<Data_Type>::Dynamic_Array(const Data_Type* data, std::size_t Size)
{
    FUNCTION_PROFILE_ARG(data, Size)
    metadata = static_cast<Dynamic_Array_metadata<Data_Type>*>(
        malloc(sizeof(Dynamic_Array_metadata<Data_Type>) +
               (Size * sizeof(Data_Type))));
    // initialization

    node_initialize(metadata, nullptr, nullptr, 0, Size);
    memcpy(metadata->Data, data, Size * sizeof(Data_Type));
    Max_Index = Size;
    Dynamic_Array<Data_Type> st_index = Size;
}

template <class Data_Type>
void Dynamic_Array<Data_Type>::reserve(const unsigned int Size)
{
    FUNCTION_PROFILE_ARG(Size)

    if (metadata == nullptr)
    {
        metadata = static_cast<Dynamic_Array_metadata<Data_Type>*>(
            malloc(sizeof(Dynamic_Array_metadata<Data_Type>) +
                   (Size * sizeof(Data_Type))));
        // metadata->Data = reinterpret_cast<Data_Type*>(metadata + 1);
        node_initialize(metadata, nullptr, nullptr, 0, Size);
        Max_Index = Size;
    }
    else [[likely]]

    {
        Grow(Size);
    }
}

template <class Data_Type>
const Data_Type&& Dynamic_Array<Data_Type>::operator=(const Data_Type&& data)
{
    FUNCTION_PROFILE_ARG(data)

    return data;
}

template <class Data_Type>
Data_Type& Dynamic_Array<Data_Type>::get_data_at(const std::uint64_t position)
{
    FUNCTION_PROFILE_ARG(position)

    if ((position < metadata->Base_Index + metadata->Capacity) &&
        (position >= metadata->Base_Index)) [[likely]]
    {
        // check if it is in the boundary of present data set
        return metadata->Data[position - metadata->Base_Index];
    }
    else if (position > metadata->Base_Index + metadata->Capacity)
    {
        // if it is ahead of present data set
        if (position > Max_Index) Grow((Max_Index - position) + grow_policy());

        forward_goto_position(position);
        return metadata->Data[position - metadata->Base_Index];
    }
    else
    {
        // if it is behind of present data set

        backward_goto_position(position);
        return metadata->Data[position - metadata->Base_Index];
    }
}
template <class Data_Type>
Data_Type& Dynamic_Array<Data_Type>::operator[](const std::uint64_t position)
{
    FUNCTION_PROFILE_ARG(position)

    return get_data_at(position);
}

// Modifiers

// sets the contents
template <class Data_Type>
void Dynamic_Array<Data_Type>::set(const Data_Type&& data)
{
    FUNCTION_PROFILE_ARG(data);

    for (auto i = 0; i < Last_index; i++)
        get_data_at(i) = data;
}
// bool insert();

// inserts elements
// bool insert_range();

// inserts a range of elements
// bool empDynamic_Array<Data_Type>ce();

// constructs element in-pDynamic_Array<Data_Type>ce
// bool erase();

// erases elements
template <class Data_Type>
void Dynamic_Array<Data_Type>::push_back(const Data_Type& data)
{
    FUNCTION_PROFILE_ARG(data)
    Last_index += 1;

    if ((metadata->Base_Index + metadata->Capacity) > Last_index)
    {
        get_data_at(Last_index) = data;
    }
    else
    {
        Grow();
        get_data_at(Last_index) = data;
    }
}
template <class Data_Type>
const Data_Type Dynamic_Array<Data_Type>::pop_back()
{
    FUNCTION_PROFILE_ARG("")
    Last_index--;
    return get_data_at(Last_index + 1);
}

// removes the Dynamic_Array<Data_Type>st element

template <class Data_Type>
void Dynamic_Array<Data_Type>::Register_grow_policy(
    std::uint64_t (*policy)(unsigned long long, unsigned long long))
{
    FUNCTION_PROFILE
    Grow_policy = policy;
}
template <class Data_Type>

const std::uint_fast64_t Dynamic_Array<Data_Type>::Max_element()
{
    return Max_Index;
}
// linked
template <class Data_Type>
void Dynamic_Array<Data_Type>::insert(const std::uint_fast64_t position,
                                      const Data_Type&&        data,
                                      const int                Size)
{
    int*                               breakpos = get_data_at(position);
    Dynamic_Array_metadata<Data_Type>* next = metadata->Next;

    // checking if this is the Dynamic_Array<Data_Type>st element
    //
    //
    if (metadata->Base_Index + metadata->Capacity - 1 == position)

    {
        Dynamic_Array_metadata<Data_Type>* Inserted_arr =
            static_cast<Dynamic_Array_metadata<Data_Type>*>(
                malloc((sizeof(Dynamic_Array_metadata<Data_Type>)) +
                       (Size * sizeof(Data_Type))));
    }
}

}  // namespace DATA_STRUCTURE
