#pragma once

#include <types.h>

/// @brief Initialises memory and informs OS about memory
void InitMemory();

/// @brief Returns total memory of system
uint64_t GetTotalMemory();
/// @brief Returns currently free memory of system 
uint64_t GetFreeMemory();
/// @brief Returns currently used memory of system 
uint64_t GetUsedMemory();
/// @brief Returns currently reserved memory of system 
uint64_t GetReservedMemory();

/// @brief Requests a free page from memory and locks the page
/// @return Returns the address of a free page of memory
void* RequestPage();
/// @brief Frees a page
/// @param address Address of page to free
void FreePage(void* address);
/// @brief Frees multiple pages in a contiguous block
/// @param address Address of first page to free
/// @param count Number of pages to free
void FreePages(void* address, uint64_t count);
/// @brief Locks a page
/// @param address Address of page to lock
void LockPage(void* address);
/// @brief Locks multiple pages in a contiguous block
/// @param address Address of first page to lock
/// @param count Number of pages to lock
void LockPages(void* address, uint64_t count);
/// @brief Unreserves a page
/// @param address Address of page to unreserve
void UnreservePage(void* address);
/// @brief Unreserves multiple pages in a contiguous block
/// @param address Address of first page to unreserve
/// @param count Number of pages to unreserve
void UnreservePages(void* address, uint64_t count);
/// @brief Reserves a page
/// @param address Address of page to reserve
void ReservePage(void* address);
/// @brief Reserves multiple pages in a contiguous block
/// @param address Address of first page to reserve
/// @param count Number of pages to reserve
void ReservePages(void* address, uint64_t count);