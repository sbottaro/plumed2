/*
 * This file is part of the GROMACS molecular simulation package.
 *
 * Copyright (c) 1991-2000, University of Groningen, The Netherlands.
 * Copyright (c) 2001-2004, The GROMACS development team.
 * Copyright (c) 2011,2012,2013,2014,2015, by the GROMACS development team, led by
 * Mark Abraham, David van der Spoel, Berk Hess, and Erik Lindahl,
 * and including many others, as listed in the AUTHORS file in the
 * top-level source directory and at http://www.gromacs.org.
 *
 * GROMACS is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation; either version 2.1
 * of the License, or (at your option) any later version.
 *
 * GROMACS is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with GROMACS; if not, see
 * http://www.gnu.org/licenses, or write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA.
 *
 * If you want to redistribute modifications to GROMACS, please
 * consider that scientific software is very special. Version
 * control is crucial - bugs must be traceable. We will be happy to
 * consider code for inclusion in the official distribution, but
 * derived work must not be called official GROMACS. Details are found
 * in the README & COPYING files - if they are missing, get the
 * official version at http://www.gromacs.org.
 *
 * To help us fund GROMACS development, we humbly ask that you cite
 * the research papers on the package. Check out http://www.gromacs.org.
 */

#ifndef _repl_ex_h
#define _repl_ex_h

#include <cstdio>

#include "gromacs/utility/basedefinitions.h"
#include "gromacs/utility/real.h"

struct gmx_enerdata_t;
struct gmx_multisim_t;
struct t_commrec;
struct t_inputrec;
struct t_state;

/* Abstract type for replica exchange */
typedef struct gmx_repl_ex *gmx_repl_ex_t;

gmx_repl_ex_t init_replica_exchange(FILE *fplog,
                                    const gmx_multisim_t *ms,
                                    const t_state *state,
                                    const t_inputrec *ir,
                                    int nst, int nmultiex, int init_seed);
/* Should only be called on the master nodes */

gmx_bool replica_exchange(FILE *fplog,
                          const t_commrec *cr,
                          gmx_repl_ex_t re,
                          t_state *state, gmx_enerdata_t *enerd,
                          t_state *state_local,
                          gmx_int64_t step, real time);
/* Attempts replica exchange, should be called on all nodes.
 * Returns TRUE if this state has been exchanged.
 * When running each replica in parallel,
 * this routine collects the state on the master node before exchange.
 * With domain decomposition, the global state after exchange is stored
 * in state and still needs to be redistributed over the nodes.
 */

void print_replica_exchange_statistics(FILE *fplog, gmx_repl_ex_t re);
/* Should only be called on the master nodes */

/* PLUMED HREX */
extern int replica_exchange_get_repl(const gmx_repl_ex_t re);
extern int replica_exchange_get_nrepl(const gmx_repl_ex_t re);
extern void pd_collect_state(const t_commrec *cr, t_state *state);
extern void exchange_state(const gmx_multisim_t *ms, int b, t_state *state);
extern void copy_state_nonatomdata(t_state *state, t_state *state_local);
/* END PLUMED HREX */

#endif  /* _repl_ex_h */
