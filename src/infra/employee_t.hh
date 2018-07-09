/*********************************************************************
 * @file    segment_t.hh
 * @author 	Nick Weber
 * @date    Mai 07, 2018
 * @brief 	Tuple class for employees. Used for transforming to disk and memory representations
 * @bugs 	TBD
 * @todos 	TBD
 * 
 * @section	DESCRIPTION
 * TBD
 * 
 * @section USE
 * TBD
 ********************************************************************/ //TODO
#pragma once

#include "tuple.hh"
#include "types.hh"
#include "exception.hh"
#include "trace.hh"

#include <cmath>
#include <cstring>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

class Employee_T : public Tuple
{
    public:
        Employee_T();
        Employee_T(const std::string& aName, const double aSalary, const uint8_t aAge); 
        Employee_T(const Employee_T& aEmployeeTuple);
        explicit Employee_T(Employee_T&&) = delete;
        Employee_T& operator=(const Employee_T& aEmployeeTuple);
        Employee_T& operator=(Employee_T&&) = delete;
        ~Employee_T() = default;

    public:
        /** TODO
         * @brief 
         * 
         * @param aName 
         * @param aSalary 
         * @param aAge 
         */
        void init(const std::string& aName, const double aSalary, const uint8_t aAge) noexcept;
        /** TODO
         * @brief 
         * 
         * @param aPtr 
         */
        void toDisk(byte* aPtr) const noexcept override;
        void toDisk(byte* aPtr) noexcept override;
        /** TODO
         * @brief 
         * 
         * @param aPtr 
         */
        void toMemory(byte* aPtr) noexcept override;
    
    public:
        static inline string_vt attributes() noexcept { return {"ID", "Age", "Name", "Salary"}; }
        // Getter
        inline uint64_t ID() const noexcept { return _id; }
        inline uint64_t ID() noexcept { return _id; }
        inline uint8_t age() const noexcept { return _age; }
        inline uint8_t age() noexcept { return _age; }
        inline const std::string& name() const noexcept { return _name; }
        inline const std::string& name() noexcept { return _name; }
        inline double salary() const noexcept { return _salary; }
        inline double salary() noexcept { return _salary; }
        /** TODO
         * @brief 
         * 
         * @return std::string 
         */
        inline std::string to_string() const noexcept override; 
        inline std::string to_string() noexcept override;

        // stringstream
        inline string_vt values() const noexcept { return {std::to_string(_id), std::to_string(_age), _name, std::to_string(std::round(_salary))}; }
        
    private:
        static uint64_t _idCount;
        /* content of the tuple */
        uint64_t        _id;
        uint8_t         _age;
        std::string     _name;  
        double          _salary;        
};

std::string Employee_T::to_string() const noexcept 
{ 
    return std::string("Employee (ID, Age, Name, Salary) : ") 
        + std::to_string(_id) + std::string(", '") 
        + std::to_string(_age) + std::string(", ") 
        + _name + std::string("', ") 
        + std::to_string(_salary); 
}

std::string Employee_T::to_string() noexcept 
{ 
    return static_cast<const Employee_T&>(*this).to_string(); 
}

using emp_vt = std::vector<Employee_T>;
std::ostream& operator<< (std::ostream& stream, const Employee_T& aEmpTuple);
