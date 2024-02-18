// Payload class for testing container-like objects and memory-managing code
// author: Hartmut Schirmacher, Berliner Hochschule für Technik

#pragma once

#include <memory>
#include <string>
#include <iostream>

// forward declarations 
class Payload;
inline std::ostream& operator<<(std::ostream& os, const Payload& p);

/*
 *  Payload serves for testing correct use of constructors and destructors when
 *  designing you own memory-managing classes.
 * 
 *  - counts how often a constructor was called
 *  - counts how often a destructor was called
 *  - assigns sequential ID to each Payload object to check when it was constructed
 *  - checks if you access Payload object before construction (if ID is in wrong range)
 *  - marks memory of destructed Payload object with a special ID
 *  - checks if you access Payload object after destruction
 *
 */
class Payload {

    // the unique positive ID of this instance of Payload
    std::size_t id_;

    // the total number of Payload instances created and destroyed
    static size_t ctor_count_;
    static size_t dtor_count_;

    // is console logging currently switched on?
    static bool do_logging_; 

public:

    // custom user content attached to the payload by the user - users can read/write as they like
    std::string content; 

    // id of this Payload, read-only
    std::size_t id() const { return id_; }

    // switch console logging on/off
    static void do_logging(bool v) { do_logging_ = v; }

    // query whether logging is currently switched on 
    static bool is_logging() { return do_logging_; }

    // constructors
    Payload(std::string user_content = "no content") 
        : id_(ctor_count_ + start_id_), content(user_content)
    {
        if(do_logging_) std::cout << "Payload(" << id_ << "|" << content << ")" << std::endl;

        ctor_count_++; // one more Payload has been created
    }

    Payload(const Payload& rhs) 
        : id_(ctor_count_ + start_id_), content(rhs.content)
    {
        if(do_logging_) std::cout << "Payload-copy-ctor(" << id_ << "|" << content << ")" << std::endl;

        ctor_count_++; // one more Payload has been created
    }

    Payload(const Payload&& rhs) 
        : id_(ctor_count_ + start_id_), content(rhs.content)
    {
        if(do_logging_) 
            std::cout << "Payload-move-ctor(" << id_ << "|" << content << ")" << std::endl;

        ctor_count_++; // one more Payload has been created
    }

    // destructor
    ~Payload()  
    {
        if(do_logging_) 
            std::cout << "Payload-dtor(" << id_ << "|" << content << ")" << std::endl;

        // write special id into memory to detect when accessing this again
        id_ = destruct_id_; 

        // one Payload has been destroyed, so there is one less alive
        dtor_count_++; 
    }

    // copy assignment 
    Payload& operator=(const Payload& rhs) 
    {
        if(do_logging_) 
            std::cout << "Payload-copy-assign(" << id_ << "|" << content << ")" << std::endl;

        check_(); // detect if this Payload object was properly initialized etc.
        content = rhs.content; // copy user content
        // alive count stays the same, no object is constructed or destroyed
        return *this;
    }
 
    // move assignment 
    Payload& operator=(Payload&& rhs) 
    {
        if(do_logging_) 
            std::cout << "Payload-move-assign(" << id_ << "|" << content << ")" << std::endl;

        check_(); // was the object initialized properly?
        content = rhs.content; // copy user content, should use move() here?
        // alive count stays the same, no object is constructed or destroyed
        return *this;
    }
 
    // comparison just by user-defined content
    bool operator==(const Payload& rhs) const 
    { 
        if(do_logging_) 
            std::cout << "Payload " << *this << " == " << rhs << std::endl;

        return content == rhs.content; 
    }
    // comparison just by user-defined content
    bool operator!=(const Payload& rhs) const 
    { 
        if(do_logging_) 
            std::cout << "Payload " << *this << " != " << rhs << std::endl;

        return content != rhs.content; 
    }

    // how many instances of Payload should be alive at this point in the process?
    static int ctor_count() { return (int) ctor_count_; }

    // how many instances of Payload have been constructed over the lifetime of this process?
    static int dtor_count() { return (int) dtor_count_; }

    // how many instances of Payload have been constructed over the lifetime of this process?
    static int alive_count() { return (int)ctor_count_ - (int)dtor_count_; }

    // check if Payload seems to be a valid object; output message if not
    void check_() const
    {
        // id_ should only equal destruct_id after destructor has been called
        if(id_ == destruct_id_) {
            std::cout << "you are accessing memory of a Payload object that has already been destroyed." << std::endl;
            return;
        } 
        
        // if you called a constructor, id_ must be >= start_id
        if(id_ < start_id_) {
            std::cout << "you are accessing memory that has not yet been initialized by a Payload constructor" << std::endl;
        }
    }

private:

    static const size_t start_id_ = 100001;
    static const size_t destruct_id_ = 99999;

};

// helper function for human readable output
inline std::ostream& operator<<(std::ostream& os, const Payload& p)
{
    os << "(id=" << std::to_string(p.id()) << "|" << p.content << ")"; 
    p.check_(); // was the object initialized properly?
    return os;
}

