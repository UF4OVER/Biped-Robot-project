%IKINE560 Inverse kinematics for Puma 560 
%
%	Q = IKINE560(ROBOT, T, CONFIG)
%
% Solve the inverse kinematics of the Puma-like (spherical wristed) robot 
% ROBOT whose end-effector pose is given by T.
%
% The optional third argument specifies the configuration of the arm in
% the form of a string containing one or more of the configuration codes:
%	'l' or 'r'	lefty/righty
%	'u' or 'd'	elbow
%	'n' or 'f'	wrist flip or noflip.
%
% The default configuration is 'lun'.
%
% REFERENCE:
%
% Inverse kinematics for a PUMA 560 based on the equations by Paul and Zhang
% From The International Journal of Robotics Research
% Vol. 5, No. 2, Summer 1986, p. 32-44
%
%
% AUTHOR:
% Robert Biro		gt2231a@prism.gatech.edu
% with Gary Von McMurray
%
% GTRI/ATRP/IIMB
% Georgia Institute of Technology
% 2/13/95

% MOD HISTORY
%  4/99 use new robot object
%  4/02 tidyup, remove multiple solutions

function theta = ikine560(robot, T,configuration)

	if robot.n ~= 6,
		error('Solution only applicable for 6DOF manipulator');
	end

	if robot.mdh ~= 0,
		error('Solution only applicable for standard DH conventions');
	end
	L = robot.links;
	a1 = L{1}.A;
	a2 = L{2}.A;
	a3 = L{3}.A;

	if ~isempty( find( [L{4}.A L{5}.A L{6}.A] ~= 0 ))
		error('wrist is not spherical')
	end

	d1 = L{1}.D;
	d2 = L{2}.D;
	d3 = L{3}.D;
	d4 = L{4}.D;

	if ~ishomog(T),
		error('T is not a homog xform');
	end

	% undo base transformation
	T = inv(robot.base) * T;%�������

	% The following parameters are extracted from the Homogeneous 
	% Transformation as defined in equation 1, p. 34

	Ox = T(1,2);
	Oy = T(2,2);
	Oz = T(3,2);

	Ax = T(1,3);
	Ay = T(2,3);
	Az = T(3,3);

	Px = T(1,4);
	Py = T(2,4);
	Pz = T(3,4);

	% The configuration parameter determines what n1,n2,n4 values are used
	% and how many solutions are determined which have values of -1 or +1.

	if nargin < 3,
		configuration = '';
	else
		configuration = lower(configuration);
	end

	% default configuration

	n1 = -1;	% L
	n2 = -1;	% U
	n4 = -1;	% N
	if ~isempty(findstr(configuration, 'l')),
		n1 = -1;
	end
	if ~isempty(findstr(configuration, 'r')),
		n1 = 1;
	end
	if ~isempty(findstr(configuration, 'u')),
		if n1 == 1,
			n2 = 1;
		else
			n2 = -1;
		end
	end
	if ~isempty(findstr(configuration, 'd')),
		if n1 == 1,
			n2 = -1;
		else
			n2 = 1;
		end
	end
	if ~isempty(findstr(configuration, 'n')),
		n4 = 1;
	end
	if ~isempty(findstr(configuration, 'f')),
		n4 = -1;
	end


	%
	% Solve for theta(1)
	% 
	% r is defined in equation 38, p. 39.
	% theta(1) uses equations 40 and 41, p.39, 
	% based on the configuration parameter n1
	%

	r=sqrt(Px^2 + Py^2);
	if (n1 == 1),
		theta(1)= atan2(Py,Px) + asin(d3/r);
	else
		theta(1)= atan2(Py,Px) + pi - asin(d3/r);
	end

	%
	% Solve for theta(2)
	%
	% V114 is defined in equation 43, p.39.
	% r is defined in equation 47, p.39.
	% Psi is defined in equation 49, p.40.
	% theta(2) uses equations 50 and 51, p.40, based on the configuration 
	% parameter n2
	%

	V114= Px*cos(theta(1)) + Py*sin(theta(1));
	r=sqrt(V114^2 + Pz^2);
	Psi = acos((a2^2-d4^2-a3^2+V114^2+Pz^2)/(2.0*a2*r));
	theta(2) = atan2(Pz,V114) + n2*Psi;

	%
	% Solve for theta(3)
	%
	% theta(3) uses equation 57, p. 40.
	%

	num = cos(theta(2))*V114+sin(theta(2))*Pz-a2;
	den = cos(theta(2))*Pz - sin(theta(2))*V114;
	theta(3) = atan2(a3,d4) - atan2(num, den);

	%
	% Solve for theta(4)
	%
	% V113 is defined in equation 62, p. 41.
	% V323 is defined in equation 62, p. 41.
	% V313 is defined in equation 62, p. 41.
	% theta(4) uses equation 61, p.40, based on the configuration 
	% parameter n4
	%

	V113 = cos(theta(1))*Ax + sin(theta(1))*Ay;
	V323 = cos(theta(1))*Ay - sin(theta(1))*Ax;
	V313 = cos(theta(2)+theta(3))*V113 + sin(theta(2)+theta(3))*Az;
	theta(4) = atan2((n4*V323),(n4*V313));

	%
	% Solve for theta(5)
	%
	% num is defined in equation 65, p. 41.
	% den is defined in equation 65, p. 41.
	% theta(5) uses equation 66, p. 41.
	%
	 
	num = -cos(theta(4))*V313 - V323*sin(theta(4));
	den = -V113*sin(theta(2)+theta(3)) + Az*cos(theta(2)+theta(3));
	theta(5) = atan2(num,den);

	%
	% Solve for theta(6)
	%
	% V112 is defined in equation 69, p. 41.
	% V122 is defined in equation 69, p. 41.
	% V312 is defined in equation 69, p. 41.
	% V332 is defined in equation 69, p. 41.
	% V412 is defined in equation 69, p. 41.
	% V432 is defined in equation 69, p. 41.
	% num is defined in equation 68, p. 41.
	% den is defined in equation 68, p. 41.
	% theta(6) uses equation 70, p. 41.
	%

	V112 = cos(theta(1))*Ox + sin(theta(1))*Oy;
	V132 = sin(theta(1))*Ox - cos(theta(1))*Oy;
	V312 = V112*cos(theta(2)+theta(3)) + Oz*sin(theta(2)+theta(3));
	V332 = -V112*sin(theta(2)+theta(3)) + Oz*cos(theta(2)+theta(3));
	V412 = V312*cos(theta(4)) - V132*sin(theta(4));
	V432 = V312*sin(theta(4)) + V132*cos(theta(4));
	num = -V412*cos(theta(5)) - V332*sin(theta(5));
	den = - V432;
	theta(6) = atan2(num,den);
