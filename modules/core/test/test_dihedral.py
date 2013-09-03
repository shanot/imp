import IMP
import IMP.test
import IMP.core
import StringIO
import math

class Tests(IMP.test.TestCase):
    """Tests for dihedral restraints"""

    def _setup_particles(self, system_angle, scored_angle):
        """Build a system of 4 xyz particles that form (by construction) the
           angle `system_angle`, and add a dihedral restraint with mean
           `scored_angle`."""
        model = IMP.kernel.Model()
        particles = []
        particles.append(self.create_point_particle(model, 1.0, 0.0, -1.0))
        particles.append(self.create_point_particle(model, 0.0, 0.0, -1.0))
        particles.append(self.create_point_particle(model, 0.0, 0.0, 0.0))
        particles.append(self.create_point_particle(model,
                                                    math.cos(system_angle),
                                                    math.sin(system_angle),
                                                    0.0))
        k = IMP.core.Harmonic.get_k_from_standard_deviation(.1)
        r = IMP.core.DistanceRestraint(IMP.core.Harmonic(1.0, k),
                                       particles[0], particles[1])
        model.add_restraint(r)
        r = IMP.core.DistanceRestraint(IMP.core.Harmonic(1.0, k),
                                       particles[1], particles[2])
        model.add_restraint(r)
        r = IMP.core.DistanceRestraint(IMP.core.Harmonic(1.0, k),
                                       particles[2], particles[3])
        model.add_restraint(r)
        rsr = IMP.core.DihedralRestraint(IMP.core.Harmonic(scored_angle, k),
                                         particles[0], particles[1],
                                         particles[2], particles[3])
        model.add_restraint(rsr)
        return model, rsr

    def test_score(self):
        """Check score of dihedral restraints"""
        angles = [0.25 * math.pi, 0.3 * math.pi, 0.6 * math.pi, 0.75 * math.pi]
        for i in range(len(angles)):
            # Score of model with the same angle as the scoring function's mean
            # should be zero:
            model, rsr = self._setup_particles(angles[i], angles[i])
            self.assertLess(model.evaluate(False), 1e-6)
            # When the angle is different, score should be far from zero:
            model, rsr = self._setup_particles(angles[i], angles[-i-1])
            self.assertGreater(model.evaluate(False), 10.0)
            # Optimizing should reduce the score to zero:
            opt = IMP.core.ConjugateGradients()
            opt.set_model(model)
            opt.optimize(50)
            self.assertLess(opt.optimize(50), 1e-6)
            self.assertLess(model.evaluate(False), 1e-6)
if __name__ == '__main__':
    IMP.test.main()
