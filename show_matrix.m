
H= load('N576_K480_GF64_matrix.txt');

figure(1);
colormap (colorcube(65));
H=mod(H,66);
image(H);

set(gca,'xtick',[]);
set(gca,'ytick',[]);
set(gca,'position',[0 0 1 1]);
%axis off;
%ylabel('H=     ','Fontsize',30,'rotation',0);
%colorbar;


