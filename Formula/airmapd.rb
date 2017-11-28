class Airmapd < Formula
  homepage "https://github.com/airmap/airmapd"
  url "https://github.com/airmap/airmapd.git"
  version "0.0.1"

  depends_on "cmake" => :build
  depends_on "boost"
  depends_on "c-ares"
  depends_on "pkgconfig"
  depends_on "openssl"
  depends_on "qt@5.7"
  depends_on "zlib"

  def install
    Dir.mkdir("build")
    Dir.chdir("build")
    system "cmake","-DCMAKE_INSTALL_PREFIX=#{prefix}","-DCMAKE_PREFIX_PATH=/usr/local/Cellar/qt@5.7/5.7.1",".."
    system "make"
    system "make", "install"
  end

end
