find_library( ssl_LIBRARY_STATIC libssl.a  ssleay32.lib HINTS "${PROJECT_SOURCE_DIR}/dependency/openssl" "${PROJECT_SOURCE_DIR}/dependency/openssl" "/usr/local/opt/openssl/lib" "/usr/lib" "/usr/local/lib" "/opt/local/lib" )
find_library( crypto_LIBRARY_STATIC libcrypto.a libeay32.lib HINTS "${PROJECT_SOURCE_DIR}/dependency/openssl" "${PROJECT_SOURCE_DIR}/dependency/openssl" "/usr/local/opt/openssl/lib" "/usr/lib" "/usr/local/lib" "/opt/local/lib" )

find_library( ssl_LIBRARY_SHARED  ssleay32_shared HINTS "${PROJECT_SOURCE_DIR}/dependency/openssl" "${PROJECT_SOURCE_DIR}/dependency/openssl" "/usr/local/opt/openssl/lib" "/usr/lib" "/usr/local/lib" "/opt/local/lib" )
find_library( crypto_LIBRARY_SHARED  libeay32_shared HINTS "${PROJECT_SOURCE_DIR}/dependency/openssl" "${PROJECT_SOURCE_DIR}/dependency/openssl" "/usr/local/opt/openssl/lib" "/usr/lib" "/usr/local/lib" "/opt/local/lib" )

find_path( ssl_INCLUDE openssl/ssl.h HINTS "${PROJECT_SOURCE_DIR}/dependency/openssl" "${PROJECT_SOURCE_DIR}/dependency/openssl/include" "/usr/local/opt/openssl/include" "/usr/include" "/usr/local/include" "/opt/local/include" )

if ( ssl_LIBRARY_STATIC AND ssl_LIBRARY_SHARED AND crypto_LIBRARY_STATIC AND crypto_LIBRARY_SHARED )
    set( OPENSSL_FOUND TRUE )

    message( STATUS "Found OpenSSL include at: ${ssl_INCLUDE}" )
    message( STATUS "Found OpenSSL library at: ${ssl_LIBRARY_STATIC}" )
    message( STATUS "Found OpenSSL library at: ${ssl_LIBRARY_SHARED}" )
    message( STATUS "Found Crypto library at: ${crypto_LIBRARY_STATIC}" )
    message( STATUS "Found Crypto library at: ${crypto_LIBRARY_SHARED}" )
else ( )
    message( STATUS "Found OpenSSL include at: ${ssl_INCLUDE}" )
    message( STATUS "Found OpenSSL library at: ${ssl_LIBRARY_STATIC}" )
    message( STATUS "Found OpenSSL library at: ${ssl_LIBRARY_SHARED}" )
    message( STATUS "Found Crypto library at: ${crypto_LIBRARY_STATIC}" )
    message( STATUS "Found Crypto library at: ${crypto_LIBRARY_SHARED}" )
    message( FATAL_ERROR "Failed to locate OpenSSL dependency. see restbed/dependency/openssl; ./config shared; make all" )
endif ( )
