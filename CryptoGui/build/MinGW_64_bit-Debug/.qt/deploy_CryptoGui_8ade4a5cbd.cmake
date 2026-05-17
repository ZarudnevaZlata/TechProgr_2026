include("C:/Users/zarud/OneDrive/Desktop/prog/CryptoGui/build/MinGW_64_bit-Debug/.qt/QtDeploySupport.cmake")
include("${CMAKE_CURRENT_LIST_DIR}/CryptoGui-plugins.cmake" OPTIONAL)
set(__QT_DEPLOY_I18N_CATALOGS "qtbase")

qt6_deploy_runtime_dependencies(
    EXECUTABLE "C:/Users/zarud/OneDrive/Desktop/prog/CryptoGui/build/MinGW_64_bit-Debug/CryptoGui.exe"
    GENERATE_QT_CONF
)
