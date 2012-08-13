win32 {
    BOOST = $$(BOOST_ROOT)
    isEmpty( BOOST ) {
      BOOST = C:/Boost
    }
    INCLUDEPATH += $${BOOST}/include/boost-1_50
    LIBS += -L$${BOOST}/lib
} else {
    INCLUDEPATH += /usr/local/include
    LIBS += -L/usr/local/lib
}
