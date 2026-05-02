# SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

target_sources(
    beman.iterator_interface
    PUBLIC
        FILE_SET HEADERS
            FILES
                config.hpp
                iterator_interface.hpp
                iterator_interface_access.hpp
                detail/stl_interfaces/config.hpp
                detail/stl_interfaces/fwd.hpp
                detail/stl_interfaces/iterator_interface.hpp
                "${PROJECT_BINARY_DIR}/include/beman/iterator_interface/config_generated.hpp"
)
