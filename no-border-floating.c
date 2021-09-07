if (((nexttiled(c->mon->clients) == c && !nexttiled(c->next))
    || &monocle == c->mon->lt[c->mon->sellt]->arrange)
    && !c->isfullscreen && !c->isfloating
    && NULL != c->mon->lt[c->mon->sellt]->arrange) {
	c->w = wc.width += c->bw * 2;
	c->h = wc.height += c->bw * 2;
	wc.border_width = 0;
}
