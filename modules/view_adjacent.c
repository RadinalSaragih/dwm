/** Function to shift the current view to the left/right
*
* @param: "arg->i" stores the number of tags to shift right (positive value)
*          or left (negative value)
*/

void
view_adjacent(const Arg *arg)
{
	int i, curtags;
	int seltag = 0;
	Arg a;

	curtags = selmon->tagset[selmon->seltags];
	for(i = 0; i < LENGTH(tags); i++)
		if(curtags & (1 << i)){
			seltag = i;
			break;
		}

	seltag = (seltag + arg->i) % (int)LENGTH(tags);
	if(seltag < 0)
		seltag += LENGTH(tags);

	a.i = (1 << seltag);
	view(&a);
}
