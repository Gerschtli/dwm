{
  description = "Customized dwm";

  inputs.nixpkgs.url = "github:NixOS/nixpkgs/nixos-22.05";

  outputs = { self, nixpkgs }:
    let
      system = "x86_64-linux";

      overlay = final: prev: {
        dwm = prev.dwm.overrideAttrs (old: {
          src = builtins.path { path = ./.; name = "dwm"; };
        });
      };
    in
    {
      overlays.default = overlay;

      checks.${system}.build = (
        import nixpkgs {
          inherit system;
          overlays = [ overlay ];
        }
      ).dwm;
    };
}
