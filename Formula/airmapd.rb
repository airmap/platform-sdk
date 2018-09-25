class Airmapd < Formula
  homepage "https://github.com/airmap/platform-sdk"
  url "https://github.com/airmap/platform-sdk.git"
  version "0.0.1"

  depends_on "cmake" => :build
  depends_on "boost"
  depends_on "c-ares"
  depends_on "pkgconfig"
  depends_on "openssl"
  depends_on "qt"
  depends_on "zlib"

  def install
    Dir.mkdir("build")
    Dir.chdir("build")
    system "cmake","-DCMAKE_INSTALL_PREFIX=#{prefix}","-DCMAKE_PREFIX_PATH=/usr/local/opt/qt",".."
    system "make"
    system "make", "install"
  end

end