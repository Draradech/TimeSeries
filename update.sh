#!/bin/bash

#resample samplerate method required source target

#aggregate method required source1 source2 ... sourceN target

#graph width xmax xres xminor xmajor height ymax yres yminor ymajor source1 name color ... sourceN name color target

#./resample 1m avg 1 temp1695875946.log altaussen.1m.log
#./resample 1m avg 1 temp1695875619.log altinnen.1m.log

./resample 1m avg 1 temp1645543711.log heizung.1m.log
./resample 1m avg 1 temp1695875425.log sw.1m.log
./resample 1m avg 1 temp1695875696.log schlafz.1m.log
./resample 1m avg 1 temp1695875658.log wohnz.1m.log
./resample 1m avg 1 temp1645543834.log no.1m.log
./resample 1m avg 1 temp1679098213.log nw.1m.log
./resample 1m avg 1 temp1679098286.log essz.1m.log
./resample 1m avg 1 temp1695875591.log kuehl.1m.log
./resample 1m avg 1 temp1695875821.log gefrier.1m.log
./resample 1m avg 1 temp1695875642.log kueche.1m.log

./resample 10s avg 1 power.log power.10s.log
./resample 1m avg 5 power.10s.log power.1m.log
./resample 10m avg 5 power.1m.log power.10m.log
./resample 1h avg 5 power.10m.log power.1h.log

./resample 10m avg 5 kuehl.1m.log kuehl.10m.log
./resample 10m avg 5 gefrier.1m.log gefrier.10m.log

./aggregate avg 2 wohnz.1m.log heizung.1m.log kueche.1m.log innen.1m.log
./aggregate min 2 no.1m.log nw.1m.log sw.1m.log aussen.1m.log

./resample 10m avg 5 innen.1m.log innen.10m.log
./resample 1h avg 5 innen.10m.log innen.1h.log
./resample 24h min 120 innen.10m.log innen.24h.min.log
./resample 24h max 120 innen.10m.log innen.24h.max.log

./resample 10m avg 5 aussen.1m.log aussen.10m.log
./resample 1h avg 5 aussen.10m.log aussen.1h.log
./resample 24h min 120 aussen.10m.log aussen.24h.min.log
./resample 24h max 120 aussen.10m.log aussen.24h.max.log

./graph 1440 $(date +%s) 1m 30m 2h 500 35.0 0.1 1.0 5.0 innen.1m.log Innen ff0000 aussen.1m.log Aussen 0080ff temp.1d.pnm
./graph 1440 $(date +%s) 10m 6h 1d 500 35.0 0.1 1.0 5.0 innen.10m.log Innen ff0000 aussen.10m.log Aussen 0080ff temp.10d.pnm
./graph 1440 $(date +%s) 1h 1d 7d 500 35.0 0.1 1.0 5.0 innen.1h.log Innen ff0000 aussen.1h.log Aussen 0080ff temp.2m.pnm
./graph 1440 $(date +%s) 6h 7d 1M 500 35.0 0.1 1.0 5.0 innen.24h.min.log Innen ff0000 innen.24h.max.log "" ff0000 aussen.24h.min.log Aussen 0080ff aussen.24h.max.log "" 0080ff temp.1y.pnm

./graph 1440 $(date +%s) 1m 30m 2h 500 35.0 0.1 1.0 5.0 heizung.1m.log Heizung ff00aa \
                                                        schlafz.1m.log Schlafzimmer ff0055 \
                                                        essz.1m.log Esszimmer ff0000 \
                                                        wohnz.1m.log Wohnzimmer ff5500 \
                                                        kueche.1m.log Kueche ffaa00 \
                                                        no.1m.log NO 0080ff \
                                                        sw.1m.log SW 00d5ff \
                                                        nw.1m.log NW 00ffd5 \
                                                        temp-single.1d.pnm

./graph 1440 $(date +%s) 1m 30m 2h 500 15.0 0.1 1.0 5.0 kuehl.1m.log Kuehlschrank 0080ff gefrier.1m.log Gefrierfach 00ffd5 temp-cold.1d.pnm
./graph 1440 $(date +%s) 10m 6h 1d 500 15.0 0.1 1.0 5.0 kuehl.10m.log Kuehlschrank ff0000 gefrier.10m.log Gefrierfach 0080ff temp-cold.10d.pnm

./graph 1440 $(date +%s) 10s 10m 1h 500 2500.0 5.0 100.0 500.0 power.10s.log Leistung ff0000 power.2h5.pnm
./graph 1440 $(date +%s) 1m 30m 2h 500 2500.0 5.0 100.0 500.0 power.1m.log Leistung ff0000 power.1d.pnm
./graph 1440 $(date +%s) 10m 6h 1d 500 2500.0 5.0 100.0 500.0 power.10m.log Leistung ff0000 power.10d.pnm
./graph 1440 $(date +%s)  1h 1d 7d 500 2500.0 5.0 100.0 500.0 power.1h.log Leistung ff0000 power.2m.pnm
