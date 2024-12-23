﻿// This implementation is based on C++ standards committee paper N4189.

#include <memory>
#include <utility>

//#define UNIQUE_RESOURCE_NOEXCEPT_NOEXCEPT_THIS_RESET noexcept( noexcept( this->reset() ) )
//#define UNIQUE_RESOURCE_NOEXCEPT_NOEXCEPT_THIS_DELETER_CALL noexcept( noexcept( this->get_deleter()( resource ) ) )

namespace std {

    template< typename R, typename D >
    class unique_resource
    {
        R resource;
        D deleter;
        bool execute_on_destruction;   // exposition only
        unique_resource &operator=( unique_resource const & ) = delete;
        unique_resource( unique_resource const & ) = delete;   // no copies!
    public:
        // construction
        explicit unique_resource( R &&resource_, D &&deleter_, bool shouldrun = true ) noexcept
                : resource( std::move( resource_ ) )
                , deleter( std::move( deleter_ ) )
                , execute_on_destruction { shouldrun }
        {
        }

        // move
        unique_resource( unique_resource &&other ) noexcept
                : resource( std::move( other.resource ) )
                , deleter( std::move( other.deleter ) )
                , execute_on_destruction { other.execute_on_destruction }
        {
            other.release();
        }

        unique_resource &operator=( unique_resource &&other ) noexcept
        {
            this->reset();
            this->deleter = std::move( other.deleter );
            this->resource = std::move( other.resource );
            this->execute_on_destruction = other.execute_on_destruction;
            other.release();
            return *this;
        }

        // resource release
        ~unique_resource() noexcept
        {
            this->reset();
        }

        void reset() noexcept
        {
            if( execute_on_destruction ) {
                this->execute_on_destruction = false;
                this->get_deleter()( resource );
            }
        }

        void reset( R &&newresource ) noexcept
        {
            this->reset();
            this->resource = std::move( newresource );
            this->execute_on_destruction = true;
        }

        R const &release() noexcept
        {
            this->execute_on_destruction = false;
            return this->get();
        }

        // resource access
        R const &get() const noexcept
        {
            return this->resource;
        }

        operator R const &() const noexcept
        {
            return this->resource;
        }

        R operator->() const noexcept
        {
            return this->resource;
        }

        typename std::add_lvalue_reference< typename std::remove_pointer< R >::type >::type operator*() const
        {
            return *this->resource;
        }

        // deleter access
        const D &get_deleter() const noexcept
        {
            return this->deleter;
        }
    };

    // factories
    template< typename R, typename D >
    unique_resource< R, typename std::remove_reference< D >::type > make_unique_resource( R &&r, D &&d ) noexcept
    {
        return unique_resource< R, typename std::remove_reference< D >::type >(
            std::move( r ), std::forward< typename std::remove_reference< D >::type >( d ), true );
    }

    template< typename R, typename D >
    unique_resource< R, D > make_unique_resource_checked( R r, R invalid, D d ) noexcept
    {
        bool shouldrun = not bool( r == invalid );
        return unique_resource< R, D >( std::move( r ), std::move( d ), shouldrun );
    }
}
