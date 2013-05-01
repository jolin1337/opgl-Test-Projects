
	GLfloat D = cpos.distance(&cpos2),
			W = (cube.dimension() + cube2.dimension())/2;
	float bound = 6.0;
	//if(time_%100 < 10)
	//	gravity = Vector3(rand()%10-5, rand()%10-5, rand()%10-5);
	cvelocity.add(&gravity);
	Vector3 s = cvelocity;
	s.mult(-mass/1000).add(&cpos2);
	bool collides = true;
	if(cpos2.x < -bound && cvelocity.x > 0.0f){
		cvelocity.x = -abs(cvelocity.x)*0.5;
		gravity.x = -abs(gravity.x);
	}
	else if(cpos2.x > bound && cvelocity.x < 0.0f){
		cvelocity.x = abs(cvelocity.x)*0.5;
		gravity.x = abs(gravity.x);
	}
	else if(cpos2.y < -bound && cvelocity.y > 0.0f){
		cvelocity.y = -abs(cvelocity.y)*0.5;
		gravity.y = -abs(gravity.y);
	}
	else if(cpos2.y > bound && cvelocity.y < 0.0f){
		cvelocity.y = abs(cvelocity.y)*0.5;
		gravity.y = abs(gravity.y);
	}
	else if(cpos2.z < -bound && cvelocity.z > 0.0f){
		cvelocity.z = -abs(cvelocity.z)*0.5;
		gravity.z = -abs(gravity.z);
	}
	else if(cpos2.z > bound && cvelocity.z < 0.0f){
		cvelocity.z = abs(cvelocity.z)*0.5;
		gravity.z = abs(gravity.z);
	}
	else{
		collides = false;
	}
	

	/*if(D < W && !collides){
		//cube2.setPosition(Vector3(s.x,2.0, s.z));// collide
		cvelocity.mult(-1);
	}
	else if(D > sqrt(3)*W && !collides)
		cube2.setPosition(s);
	else */
	if(!collides){// cube == a, cube2 == b
		//cube.max.add(0.00958);
		//cube.min.add(-0.00958);
		if(cpos.x + cube.min.x <= cpos2.x + cube2.max.x && cpos.x + cube.max.x >= cpos2.x + cube2.min.x && 
			cpos.z + cube.min.z <= cpos2.z + cube2.max.z && cpos.z + cube.max.z >= cpos2.z + cube2.min.z && 
			cpos.y + cube.min.y <= cpos2.y + cube2.max.y && cpos.y + cube.max.y >= cpos2.y + cube2.min.y){
			//gravity.mult(-1);
			/*cvelocity.x = -(cvelocity.x)*0.7;
			s = cvelocity;
			s.mult(-mass/1000).add(&cpos2);
			cube2.setPosition(s);*/
        	collides = true;
			cvelocity.x = cvelocity.y = cvelocity.z = 0.0f;
		}
		/*for (int i = 0; i < cube.size(); i++) {
			Vector3 s2 = cvelocity + gravity;
			s2.mult(-mass/1000);
			Vector3 p = (v[i].position + cpos - cpos2);
			//	printf("KOKO: %f, max: %f, min: %f\n", p.z, cube2.max.z, cube2.min.z);
			GLfloat dx1 = cube2.max.x - p.x,
					dx2 = p.x - cube2.min.x,
					dy1 = cube2.max.y - p.y,
					dy2 = p.y - cube2.min.y,
					dz1 = cube2.max.z - p.z,
					dz2 = p.z - cube2.min.z;
			if(dx1 * dx2 >= -0.0f && dy1 * dy2 >= -0.0f && dz1 * dz2 >= -0.0f){
				//s = gravity;// + Vector3(0.0,cvelocity.y*2,0.0);
				//s.normalize().mult(-W);
				//s.mult(mass/100).add(&cpos2);
				p = p - s2;
				dx1 = cube2.max.x - p.x;
				dx2 = p.x - cube2.min.x;
				dy1 = cube2.max.y - p.y;
				dy2 = p.y - cube2.min.y;
				dz1 = cube2.max.z - p.z;
				dz2 = p.z - cube2.min.z;
				if(dx1 * dx2 >= -0.0f && dy1 * dy2 >= -0.0f && dz1 * dz2 >= -0.0f){
					collides = true;// collide
					cvelocity.mult(-0.7);
					//gravity = gravity * -1;
					s2 = cvelocity + gravity;
					s2.mult(-mass/1000);
					s2.add(&cpos2);
					//cube2.setPosition(s2);
					//if((cpos2 - cpos).)
				}
			}
		}*/ /*
		if(!collides)
			for (int i = 0; i < cube2.size(); i++) {
				Vector3 s2 = cvelocity;
				s2.mult(-mass/1000);
				Vector3 p = (v[i].position + cpos2 - cpos);
				//	printf("KOKO: %f, max: %f, min: %f\n", p.z, cube2.max.z, cube2.min.z);
				GLfloat dx1 = cube.max.x - p.x,
						dx2 = p.x - cube.min.x,
						dy1 = cube.max.y - p.y,
						dy2 = p.y - cube.min.y,
						dz1 = cube.max.z - p.z,
						dz2 = p.z - cube.min.z;
				if(dx1 * dx2 >= -0.0f && dy1 * dy2 >= -0.0f && dz1 * dz2 >= -0.0f){
					collides = true;// collide
					//s = gravity;// + Vector3(0.0,cvelocity.y*2,0.0);
					//s.normalize().mult(-W);
					//s.mult(mass/100).add(&cpos2);

					cvelocity.mult(-1);
				}
			}*/
		
		if(!collides)
			cube2.setPosition(s);
		//else
		//	cube2.setPosition(Vector3(cpos2.x,2.0,cpos2.z));
	}