{
  description = "A Nix-flake-based C/C++ development environment";
  inputs.nixpkgs.url = "https://flakehub.com/f/NixOS/nixpkgs/0.1";
  outputs = inputs: let
    supportedSystems = ["x86_64-linux" "aarch64-linux" "x86_64-darwin" "aarch64-darwin"];
    forEachSupportedSystem = f:
      inputs.nixpkgs.lib.genAttrs supportedSystems (system:
        f {
          pkgs = import inputs.nixpkgs {inherit system;};
        });
  in {
    devShells = forEachSupportedSystem ({pkgs}: let
      stdenv = pkgs.clangStdenv;
    in {
      default =
        pkgs.mkShell.override
        {
          inherit stdenv;
        }
        {
          packages = with pkgs;
            [
              gcc
              clang-tools
              libbsd
              # X11 libraries and development headers
              xorg.libX11
              xorg.libX11.dev
              # X11 extensions (required for XShm.h and others)
              xorg.libXext
              xorg.libXext.dev
              xorg.xorgproto  # Contains X11 protocol headers
            ]
            ++ (
              if system == "aarch64-darwin"
              then []
              else [gdb]
            );
          env = {
            CLANGD_FLAGS = "--query-driver=${pkgs.lib.getExe stdenv.cc}";
            # Make X11 libraries and headers available
            CFLAGS = "-I${pkgs.xorg.libX11.dev}/include -I${pkgs.xorg.xorgproto}/include";
            LDFLAGS = "-L${pkgs.xorg.libX11}/lib -L${pkgs.xorg.libXext}/lib -lX11 -lXext";
          };
          shellHook = ''
            echo "X11 development environment loaded"
            echo "CFLAGS and LDFLAGS are set for X11"
            echo "You can now compile with: cc \$CFLAGS \$LDFLAGS your_program.c"
            
            # Export X11 path for minilibX configure script (includes extensions)
            export NIX_X11_INCLUDE="${pkgs.xorg.xorgproto}/include"
            echo "X11 headers (including extensions) available at: $NIX_X11_INCLUDE"
            echo "libX11 headers available at: ${pkgs.xorg.libX11.dev}/include"
          '';
        };
    });
  };
}
