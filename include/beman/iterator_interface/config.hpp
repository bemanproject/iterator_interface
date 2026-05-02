#ifndef BEMAN_ITERATOR_INTERFACE_CONFIG_HPP
#define BEMAN_ITERATOR_INTERFACE_CONFIG_HPP

#if !defined(__has_include) || __has_include(<beman/iterator_interface/config_generated.hpp>)
    #include <beman/iterator_interface/config_generated.hpp>
#else
    #include <version>
    #if defined(__cpp_explicit_this_parameter) && __cpp_explicit_this_parameter >= 202110L
        #define BEMAN_ITERATOR_INTERFACE_USE_DEDUCING_THIS() 1
    #else
        #define BEMAN_ITERATOR_INTERFACE_USE_DEDUCING_THIS() 0
    #endif
#endif

#endif
