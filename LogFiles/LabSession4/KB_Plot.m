B = [0,1e-5,5e-5,1e-4,5e-4,1e-3,5e-3,1e-2];
K1 = [0.1,0.2,0.25,0.25,0.35,0.2,0,0];
K2 = [0.05,0.09,0.06,0.06,0.2,0.25,0.08,0];
K3 = [0.01,0.01,0.025,0.03,0.14,0.15,0.1,0];

plt = plot(B,K1,'LineWidth',2);
hold on
plot(B,K2,'LineWidth',2)
plot(B,K3,'LineWidth',2)
grid on

xlabel('B [Nm/(deg/s)]');
ylabel('K [Nm/deg]');
legend('Cutoff frequency 10 Hz','Cutoff frequency 50 Hz','Cutoff frequency 100 Hz')

Zwidth1 = trapz(B,K1);
Zwidth2 = trapz(B,K2);
Zwidth3 = trapz(B,K3);

saveas(plt,'KB1.png')