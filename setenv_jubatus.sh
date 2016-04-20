export JUBATUS_HOME="/home/yukimori/work/jubatus"
# jubatus-coreのライブラリを配置
export JUBATUS_CORE_HOME="${JUBATUS_HOME}/core"
# jubatusのライブラリを配置
# jubatus-coreを直接呼び出す場合は実際には配置しなくても動作する
export JUBATUS_SERVER_HOME="${JUBATUS_HOME}/server"

core_version="0.2.8"
server_version="0.8.8"


#${JUBATUS_CORE_HOME}/libは手動で集めてきたライブラリを配置
export LD_LIBRARY_PATH="${JUBATUS_CORE_HOME}/${core_version}/lib:${JUBATUS_SERVER_HOME}/${server_version}/lib:${JUBATUS_CORE_HOME}/lib"
export LDFLAGS="-L${JUBATUS_CORE_HOME}/${core_version}/lib -L${JUBATUS_SERVER_HOME}/${server_version}/lib -L${JUBATUS_CORE_HOME}/lib"

# 以下は不要であればコメントアウトすること
export CPLUS_INCLUDE_PATH="${JUBATUS_CORE_HOME}/${core_version}/include:${JUBATUS_SERVER_HOME}/${server_version}/include:${JUBATUS_HOME}/include"
export PATH="${JUBATUS_CORE_HOME}/${core_version}/bin:${JUBATUS_SERVER_HOME}/${server_version}/bin:${PATH}"
export PKG_CONFIG_PATH="${JUBATUS_CORE_HOME}/${core_version}/lib/pkgconfig"

#export JUBATUS_HOME="/opt/jubatus"
#export PATH="${JUBATUS_HOME}/bin:${PATH}"
#export LD_LIBRARY_PATH="${JUBATUS_HOME}/lib:${LD_LIBRARY_PATH}"
#export LDFLAGS="-L${JUBATUS_HOME}/lib ${LDFLAGS}"
#export CPLUS_INCLUDE_PATH="${JUBATUS_HOME}/include:${CPLUS_INCLUDE_PATH}"
#export PKG_CONFIG_PATH="${JUBATUS_HOME}/lib/pkgconfig:${PKG_CONFIG_PATH}"
#export MANPATH="${JUBATUS_HOME}/share/man:${MANPATH}"

# embedded-jubatusではこのファイルを見つける必要がある
#/home/vagrant/jubatus/core/0.2.8/include/jubatus/core_config.hpp
#/home/vagrant/jubatus/server/0.8.8/include/jubatus/config.hpp


